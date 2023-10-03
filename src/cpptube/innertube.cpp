#include <cpptube/definitions.hpp>
#include <cpptube/innertube.hpp>
#include <cpptube/helpers.hpp>
#include <cpptube/request.hpp>

#include <fstream>
#include <ctime>
#include <iostream>

namespace innertube
{
	const std::string __client_id = "861556708454-d6dlm3lh05idd8npek18k6be8ba3oc68.apps.googleusercontent.com";
	const std::string __client_secret = "SboVhoG9s0rNafixCSGGKXAT";

	const std::string __api_keys[] = {
		"AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8",
		"AIzaSyCtkvNIR1HCEwzsqK6JuE6KqpyjusIRI30",
		"AIzaSyA8eiZmM1FaDVjRy-df2KTyQ_vz_yYM39w",
		"AIzaSyC8UYZpvA2eknNex0Pjid0_eTLJoDu6los",
		"AIzaSyCjc_pVEDi4qsv5MtC2dMXzpIaDoRFLsxw",
		"AIzaSyDHQ9ipnphqTzDqZsbtd8_Ru4_kiKVQe2k"
	};

	const nlohmann::json __default_clients = R"(
	{
		"WEB": {
			"header": {
				"User-Agent": "Mozilla/5.0"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "2.20200720.00.02", 
					"clientName": "WEB"
				}
			}
		}, 
		"IOS_MUSIC": {
			"header": {
				"User-Agent": "com.google.ios.youtubemusic/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "5.21", 
					"clientName": "IOS_MUSIC", 
					"deviceModel": "iPhone14,3"
				}
			}
		}, 
		"TV_EMBED": {
			"header": {
				"User-Agent": "Mozilla/5.0"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "2.0", 
					"clientName": "TVHTML5_SIMPLY_EMBEDDED_PLAYER"
				}
			}
		}, 
		"ANDROID": {
			"header": {
				"User-Agent": "com.google.android.youtube/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"androidSdkVersion": 30, 
					"clientVersion": "17.31.35", 
					"clientName": "ANDROID"
				}
			}
		}, 
		"WEB_MUSIC": {
			"header": {
				"User-Agent": "Mozilla/5.0"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "1.20220727.01.00", 
					"clientName": "WEB_REMIX"
				}
			}
		}, 
		"IOS_CREATOR": {
			"header": {
				"User-Agent": "com.google.ios.ytcreator/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "22.33.101", 
					"clientName": "IOS_CREATOR", 
					"deviceModel": "iPhone14,3"
				}
			}
		}, 
		"WEB_CREATOR": {
			"header": {
				"User-Agent": "Mozilla/5.0"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "1.20220726.00.00", 
					"clientName": "WEB_CREATOR"
				}
			}
		}, 
		"ANDROID_MUSIC": {
			"header": {
				"User-Agent": "com.google.android.apps.youtube.music/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"androidSdkVersion": 30, 
					"clientVersion": "5.16.51", 
					"clientName": "ANDROID_MUSIC"
				}
			}
		}, 
		"ANDROID_CREATOR": {
			"header": {
				"User-Agent": "com.google.android.apps.youtube.creator/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"androidSdkVersion": 30, 
					"clientVersion": "22.30.100", 
					"clientName": "ANDROID_CREATOR"
				}
			}
		}, 
		"ANDROID_EMBED": {
			"header": {
				"User-Agent": "com.google.android.youtube/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"androidSdkVersion": 30, 
					"clientVersion": "17.31.35", 
					"clientName": "ANDROID_EMBEDDED_PLAYER", 
					"clientScreen": "EMBED"
				}
			}
		}, 
		"IOS": {
			"header": {
				"User-Agent": "com.google.ios.youtube/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "17.33.2", 
					"clientName": "IOS", 
					"deviceModel": "iPhone14,3"
				}
			}
		}, 
		"WEB_EMBED": {
			"header": {
				"User-Agent": "Mozilla/5.0"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "2.20210721.00.00", 
					"clientName": "WEB_EMBEDDED_PLAYER", 
					"clientScreen": "EMBED"
				}
			}
		}, 
		"IOS_EMBED": {
			"header": {
				"User-Agent": "com.google.ios.youtube/"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "17.33.2", 
					"clientName": "IOS_MESSAGES_EXTENSION", 
					"deviceModel": "iPhone14,3"
				}
			}
		}, 
		"MWEB": {
			"header": {
				"User-Agent": "Mozilla/5.0"
			}, 
			"api_key": "AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8", 
			"context": {
				"client": {
					"clientVersion": "2.20220801.00.00", 
					"clientName": "MWEB"
				}
			}
		}
	}
	)"_json;

	unsigned __token_timeout = 1800;
	const fs::path __cache_dir = fs::temp_directory_path();
	const fs::path __token_file = __cache_dir / "innertube_tokens.json";

	InnerTube::InnerTube(
		std::string client,
		bool use_oauth,
		bool allow_cache
	) : __use_oauth{use_oauth}, __allow_cache{allow_cache} {
		this->__context = __default_clients[client]["context"];
		this->__header = __default_clients[client]["header"];
		this->__api_key = __default_clients[client]["api_key"].get<std::string>();
		this->__access_token.clear();
		this->__refresh_token.clear();
		this->__expires = 0;

		if (this->__use_oauth && this->__allow_cache)
		{
			if (fs::exists(__token_file))
			{
				std::ifstream token_file(__token_file);
				nlohmann::json data;
				token_file >> data;
				token_file.close();
				this->__access_token = data["access_token"].get<std::string>();
				this->__refresh_token = data["refresh_token"].get<std::string>();
				this->__expires = data["expires"].get<unsigned>();
				this->refresh_bearer_token();
			}
		}
	}

	void InnerTube::cache_tokens()
	{
		if (!this->__allow_cache)
			return;

		nlohmann::json data = {
			{"access_token", this->__access_token},
			{"refresh_token", this->__refresh_token},
			{"expires", this->__expires}
		};

		if (!fs::exists(__cache_dir))
		{
			fs::create_directories(__cache_dir);
		}

		std::ofstream token_file(__token_file);
		token_file << data;
		token_file.close();
	}

	void InnerTube::refresh_bearer_token(bool force)
	{
		if (!this->__use_oauth)
			return;

		if ((this->__expires > time(NULL)) && (!force))
			return;

		unsigned start_time = unsigned(time(NULL) - 30);
		std::string data = nlohmann::json{
			{"client_id", __client_id},
			{"client_secret", __client_secret},
			{"grant_type", "refresh_token"},
			{"refresh_token", this->__refresh_token},
		}.dump();
		std::string response = cpptube::request::execute_request(
			"https://oauth2.googleapis.com/token",
			METHOD_POST,
			{
				{"Content-Type", "application/json"}
			},
			(const std::string*)&data
		);
		nlohmann::json response_data = nlohmann::json::parse(response);

		this->__access_token = response_data["access_token"].get<std::string>();
		this->__expires= response_data["expires_in"].get<unsigned>();
		this->cache_tokens();
	}

	void InnerTube::fetch_bearer_token()
	{

	}

	std::string InnerTube::base_url()
	{
		return "https://www.youtube.com/youtubei/v1";
	}

	nlohmann::json InnerTube::base_data()
	{
		return {
			{"context", this->__context}
		};
	}

	nlohmann::json InnerTube::base_params()
	{
		return {
			{"key", this->__api_key},
			{"contentCheckOk", true},
			{"racyCheckOk", true},
		};
	}

	nlohmann::json InnerTube::__call_api(const std::string& endpoint, nlohmann::json query, const std::string& data)
	{
		if (this->__use_oauth)
			query["key"].clear();

		std::string endpoint_url = endpoint + "?" + cpptube::helpers::url_encode(query);
		nlohmann::json headers = {
			{"Content-Type", "application/json"}
		};

		if (this->__use_oauth)
		{
			if (!this->__access_token.empty())
			{
				this->refresh_bearer_token();
			}
			else
			{
				this->fetch_bearer_token();
			}
			headers["Authorization"] = "Bearer " + this->__access_token;
		}

		headers.merge_patch(this->__header);
		return nlohmann::json::parse(cpptube::request::execute_request(
			endpoint_url,
			METHOD_POST,
			headers,
			&data
		));
	}

	nlohmann::json InnerTube::player(const std::string& video_id)
	{
		std::string endpoint = this->base_url() + "/player";
		nlohmann::json query = {
			{"videoId", video_id},
		};
		query.merge_patch(this->base_params());
		return this->__call_api(endpoint, query, this->base_data().dump());
	}
}
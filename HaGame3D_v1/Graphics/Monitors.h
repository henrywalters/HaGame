#ifndef MONITORS
#define MONITORS

#include <SDL.h>
#include "../Utils/Aliases.h"


namespace hagame {
	namespace graphics {

		struct Monitor {
			int id;
			Rect displayBounds;
		};

		struct MonitorConfig {
			Monitor monitor;
			int modeId;
			Vec2 size;
			int refreshRate;
			SDL_DisplayMode* _dm;
		};

		class MonitorManager {
		private:

			static MonitorConfig BuildConfig(int monitorId, int modeId, SDL_DisplayMode* dm) {
				return MonitorConfig{ Monitor{ monitorId }, modeId, Vec2({(float) dm->w, (float) dm->h}), dm->refresh_rate, dm };
			}

		public:

			static int GetAvailableMonitorCount() {
				return SDL_GetNumVideoDisplays();
			}

			static Monitor GetMonitor(int monitorId) {
				return Monitor{ monitorId, GetDisplayBounds(monitorId) };
			}

			static Array<Monitor> GetMonitors() {
				Array<Monitor> monitors = Array<Monitor>();
				for (int i = 0; i < GetAvailableMonitorCount(); i++) {
					monitors.push_back(GetMonitor(i));
				}
				return monitors;
			}

			static int GetAvailableConfigCount(int monitorId) {
				return SDL_GetNumDisplayModes(monitorId);
			}

			static MonitorConfig GetDesktopConfig(int monitorId) {
				SDL_DisplayMode dm;
				SDL_GetDesktopDisplayMode(monitorId, &dm);
				return BuildConfig(monitorId, 0, &dm);
			}

			static MonitorConfig GetConfig(int monitorId, int modeId) {
				SDL_DisplayMode dm;
				SDL_GetDisplayMode(monitorId, modeId, &dm);
				return BuildConfig(monitorId, modeId, &dm);
			}

			static MonitorConfig GetCurrentConfig(int monitorId) {
				SDL_DisplayMode dm;
				SDL_GetCurrentDisplayMode(monitorId, &dm);
				return BuildConfig(monitorId, 0, &dm);
			}

			static Rect GetDisplayBounds(int monitorId) {
				SDL_Rect rect;
				SDL_GetDisplayBounds(monitorId, &rect);
				return Rect(Vec2({ (float) rect.x, (float) rect.y }), Vec2({ (float) rect.w, (float)rect.h }));
			}

			static Array<MonitorConfig> GetAvailableConfigs(int monitorId) {
				auto configs = Array<MonitorConfig>();
				for (int i = 0; i < GetAvailableConfigCount(monitorId); i++) {
					configs.push_back(GetConfig(monitorId, i));
				}
				return configs;
			}
		};
	}
}

#endif
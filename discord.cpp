#include "Discord.hpp"
#include <chrono>

void Discord::Initialize() {
	DiscordEventHandlers handle;
	memset(&handle, 0, sizeof(handle));
	Discord_Initialize("1123910633612656651", &handle, 1, NULL);
}

static int64_t eptime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

void Discord::Update() {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Playing";
    discordPresence.details = "Undetected Valorant Chair :)";
    discordPresence.startTimestamp = eptime;
    discordPresence.largeImageKey = "wolf";
    discordPresence.largeImageText = "Playing Val using Valorant X";
    Discord_UpdatePresence(&discordPresence);
}
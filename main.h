
#define M_PI (double)3.1415926535
#define actor_player 4
static bool silentaimm = false;
static bool writehealth = false;
static bool ammowarning = false;
static int customwarning = 1;
static bool showammo = false;
static bool radar = false;
static bool showname = true;

static bool legitt = false;
static bool legitcheck = false;
bool genderesp = false;
bool fovchanger = false;


int AimKeyList[] = { VK_LBUTTON , VK_RBUTTON , VK_MBUTTON , VK_MENU , VK_CONTROL , VK_SHIFT , VK_XBUTTON1, VK_XBUTTON2 };

const char* optKey[] = { "Left Click", "Right Click", "MClick", "Alt", "Ctrl", "Shift", "Mouse Button 4", "Mouse Button 5" };

const char* aimbone[] = { "Head", "Chest", "Pelvis" }; //0,1,2 = bone number

const char* colors2[] = { "Red", "Black", "Cyan", "Green", "Rainbow" };


const char* type[] = { "Corner",  "2D", "3D" };


static int keyint = 0;
static int colorint = 0;


static bool recoilcontrol = false;

float skeletonfloat = 1.f;
//cheat settings for menu, etc


struct Config {

	INT aimbotkey;
	// Aimbot settings
	bool aimenable = false;
	bool visiblecheck = false;
	bool circlehead = false;
	bool rcs = false;
	bool aimbot_constant = false;
	bool aimbot_mode_silent = false;
	bool aimbot_draw_fov = false;
	bool aimbot_draw_target_line = false;
	bool aimbot_ignore_with_useless_weapons = false;
	float aimbot_fov = 20.f;
	float aimbot_smooth = 5.f;
	float aim_constant_offset = 0.f;
	float silent_fov = 1.f;
	float silent_smooth = 1.f;
	float fovchangervalue = 100.f;
	float legit_smooth = 5.f;

	// ESP settings
	bool player_box = false;
	bool player_box_3d = false;
	bool player_skeleton = false;
	bool player_snapline = false;
	bool player_view_angle = false;
	bool player_distance = false;
	bool player_healthbar = false;
	bool player_agent = false;
	bool player_weapon = false;
	bool glow = false;
	bool chams = false;
	bool characterr = false;
	bool fovchanger = false;
	bool player_ignore_dormant = false;

	int aimboness = 0;
	int bonees = 0;

	int esptype = 0;

	int healthhtype = 0;
	ImColor headboxcolor = ImColor(0, 255, 0);
	ImColor espcolor = ImColor(255, 255, 0);
	ImVec4 snapcolor = { 1.0f, 1.0f, 1.0f, 1.0f }; // snapline renk
	ImVec4 skeleton = { 1.0f, 1.0f, 1.0f, 1.0f }; // skeleton renk
};

// Sample usage
Config config;

void SaveConfig() {
	std::ofstream config_file("C:\\capwarecfg.ini");
	config_file << config.player_box << std::endl;
	config_file << config.characterr << std::endl;
	config_file << config.circlehead << std::endl;
	config_file << config.player_view_angle << std::endl;
	config_file << config.player_skeleton << std::endl;
	config_file << config.player_snapline << std::endl;
	config_file << config.player_distance << std::endl;
	config_file << config.player_healthbar << std::endl;
	config_file << config.player_ignore_dormant << std::endl;
	config_file << config.glow << std::endl;
	config_file << config.fovchanger << std::endl;
	config_file << config.fovchangervalue << std::endl;
	config_file << config.esptype << std::endl;
	config_file << config.healthhtype << std::endl;
	//aimbot
	config_file << config.aimenable << std::endl;
	config_file << config.aimbot_draw_fov << std::endl;
	config_file << config.rcs << std::endl;
	config_file << config.aimboness << std::endl;
	config_file << config.aimbotkey << std::endl;
	config_file << config.aimbot_fov << std::endl;
	config_file << config.aimbot_smooth << std::endl;
	//color
	config_file << config.headboxcolor.Value.x << " " << config.headboxcolor.Value.y << " " << config.headboxcolor.Value.z << " " << config.headboxcolor.Value.w << std::endl;
	config_file << config.espcolor.Value.x << " " << config.espcolor.Value.y << " " << config.espcolor.Value.z << " " << config.espcolor.Value.w << std::endl;
	config_file << config.snapcolor.x << " " << config.snapcolor.y << " " << config.snapcolor.z << " " << config.snapcolor.w << std::endl;
	config_file << config.skeleton.x << " " << config.skeleton.y << " " << config.skeleton.z << " " << config.skeleton.w << std::endl;
	config_file.close();
}

void LoadConfig() {
	std::ifstream config_file("C:\\capwarecfg.ini");
	//visuals
	config_file >> config.player_box;
	config_file >> config.characterr;
	config_file >> config.circlehead;
	config_file >> config.player_view_angle;
	config_file >> config.player_skeleton;
	config_file >> config.player_snapline;
	config_file >> config.player_distance;
	config_file >> config.player_healthbar;
	config_file >> config.player_ignore_dormant;
	config_file >> config.glow;
	config_file >> config.fovchanger;
	config_file >> config.fovchangervalue;
	config_file >> config.esptype;
	config_file >> config.healthhtype;
	//aimbot
	config_file >> config.aimenable;
	config_file >> config.aimbot_draw_fov;
	config_file >> config.rcs;
	config_file >> config.aimboness;
	config_file >> config.aimbotkey;
	config_file >> config.aimbot_fov;
	config_file >> config.aimbot_smooth;
	//color
	config_file >> config.headboxcolor.Value.x >> config.headboxcolor.Value.y >> config.headboxcolor.Value.z >> config.headboxcolor.Value.w;
	config_file >> config.espcolor.Value.x >> config.espcolor.Value.y >> config.espcolor.Value.z >> config.espcolor.Value.w;
	config_file >> config.snapcolor.x >> config.snapcolor.y >> config.snapcolor.z >> config.snapcolor.w;
	config_file >> config.skeleton.x >> config.skeleton.y >> config.skeleton.z >> config.skeleton.w;

	config_file.close();

}


INT legitkey;
INT legitbone;
INT silentKey;
INT silentbone;


bool IsVisible;
float silent_smooth = 5.f;



DWORD_PTR LocalPawn;
uintptr_t playerstate;
uintptr_t teamComponent;
int teamID;
uintptr_t world;
uintptr_t game_instance;
uintptr_t persistent_level;
uintptr_t local_player;
INT32 actor_count;

int customX;
int customY;

//typedef PVOID(*name)();

uintptr_t base;


typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;

class Color
{
public:
	RGBA moi_blue = { 0, 143, 209,255 };
	RGBA moi_red = { 241,34,34,255 };
	RGBA moi_green = { 0,209,91,255 };

	RGBA red = { 255,0,0,255 };
	RGBA Magenta = { 255,0,255,255 };
	RGBA yellow = { 255,255,0,255 };
	RGBA grayblue = { 128,128,255,255 };
	RGBA green = { 128,224,0,255 };
	RGBA darkgreen2 = { 26, 128, 0, 255 };
	RGBA darkgreen3 = { 6, 68, 0, 255 };
	RGBA darkgreen = { 0,224,128,255 };
	RGBA brown = { 192,96,0,255 };
	RGBA pink = { 255,168,255,255 };
	RGBA DarkYellow = { 216,216,0,255 };
	RGBA SilverWhite = { 236,236,236,255 };
	RGBA purple = { 144,0,255,255 };
	RGBA Navy = { 88,48,224,255 };
	RGBA skyblue = { 0,136,255,255 };
	RGBA graygreen = { 128,160,128,255 };
	RGBA blue = { 0,96,192,255 };

	RGBA orange = { 255,128,0,255 };
	RGBA peachred = { 255,80,128,255 };
	RGBA reds = { 255,128,192,255 };
	RGBA darkgray = { 96,96,96,255 };
	RGBA Navys = { 0,0,128,255 };
	RGBA darkgreens = { 0,128,0,255 };
	RGBA darkblue = { 0,128,128,255 };
	RGBA redbrown = { 128,0,0,255 };
	RGBA red4 = { 255, 24, 101, 255 };
	RGBA purplered = { 128,0,128,255 };
	RGBA greens = { 0,255,0,255 };
	RGBA envy = { 0,255,255,255 };
	RGBA black = { 0,0,0,255 };
	RGBA gray = { 128,128,128,255 };
	RGBA white = { 255,255,255,255 };
	RGBA blues = { 30,144,255,255 };
	RGBA lightblue = { 135,206,250,160 };
	RGBA Scarlet = { 220, 20, 60, 160 };
	RGBA white_ = { 255,255,255,200 };
	RGBA gray_ = { 128,128,128,200 };
	RGBA black_ = { 0,0,0,200 };
	RGBA red_ = { 255,0,0,200 };
	RGBA Magenta_ = { 255, 95, 96, 255 };
	RGBA yellow_ = { 255,255,0,200 };
	RGBA grayblue_ = { 128,128,255,200 };
	RGBA green_ = { 128,224,0,200 };
	RGBA darkgreen_ = { 0,224,128,200 };
	RGBA brown_ = { 192,96,0,200 };
	RGBA pink_ = { 255,168,255,200 };
	RGBA darkyellow_ = { 216,216,0,200 };
	RGBA silverwhite_ = { 236,236,236,200 };
	RGBA purple_ = { 144,0,255,200 };
	RGBA Blue_ = { 88,48,224,200 };
	RGBA skyblue_ = { 0,136,255,200 };
	RGBA graygreen_ = { 128,160,128,200 };
	RGBA blue_ = { 0,96,192,180 };
	RGBA orange_ = { 255,128,0,200 };
	RGBA orange2_ = { 255,100,0,180 };
	RGBA pinks_ = { 255,80,128,200 };
	RGBA Fuhong_ = { 255,128,192,200 };
	RGBA darkgray_ = { 96,96,96,200 };
	RGBA Navy_ = { 0,0,128,200 };
	RGBA darkgreens_ = { 0,128,0,200 };
	RGBA darkblue_ = { 0,128,128,200 };
	RGBA redbrown_ = { 128,0,0,200 };
	RGBA purplered_ = { 128,0,128,200 };
	RGBA greens_ = { 0,255,0,200 };
	RGBA envy_ = { 0,255,255,200 };

	RGBA glassblack = { 0, 0, 0, 180 };
	RGBA GlassBlue = { 65,105,225,80 };
	RGBA glassyellow = { 255,255,0,160 };
	RGBA glass = { 200,200,200,60 };


	RGBA Plum = { 221,160,221,160 };

};
Color Col;

class FLinearColor {
public:
	FLinearColor() : red(0.f), green(0.f), blue(0.f), alpha(0.f) {}
	FLinearColor(float red, float green, float blue, float alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

public:
	float red;
	float green;
	float blue;
	float alpha;
};


inline auto DrawLineNew(int x1, int y1, int x2, int y2, RGBA* color, int thickness) -> void
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}

static auto DrawRect(int x, int y, int w, int h, RGBA* color, int thickness) -> void
{
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
}

inline auto DrawFilledRect2(float x, float y, float w, float h, ImColor color) -> void
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, NULL, NULL);
}

inline auto DrawFilledRect3(int x, int y, int w, int h, RGBA* color) -> void
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
}

inline auto DrawFilledRect4(float x, float y, float w, float h, ImColor color) -> void
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, NULL, NULL);
}

inline auto DrawBorder(float x, float y, float w, float h, float px, RGBA* BorderColor) -> void
{
	DrawRect(x, (y + h - px), w, px, BorderColor, 1 / 2);
	DrawRect(x, y, px, h, BorderColor, 1 / 2);
	DrawRect(x, y, w, px, BorderColor, 1 / 2);
	DrawRect((x + w - px), y, px, h, BorderColor, 1 / 2);
}


inline auto DrawCircleFilled2(int x, int y, int radius, RGBA* color) -> void
{
	ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

//copy pasta offsets here
namespace offsets
{
	std::ptrdiff_t uworld_pointer = 0x60; //UWorld*
	std::ptrdiff_t fnamepool = 0xAC29240;		// game_base
	// world
	std::ptrdiff_t persistent_level = 0x38;			// world > persistent_level
	std::ptrdiff_t game_instance = 0x1A0;				// world > game_instance

	// player
	std::ptrdiff_t localplayers_array = 0x40;			// world > game_instance > localplayers_array
	std::ptrdiff_t localplayer = 0x40;				// world > game_instance > localplayers_array[0]
	std::ptrdiff_t player_controller = 0x38;			// world > game_instance > localplayers_array[0] > playercontroller
	std::ptrdiff_t apawn = 0x448;					// world > game_instance > localplayers_array[0] > playercontroller > apawn									// aactor > apawn

	// vector
	std::ptrdiff_t root_component = 0x238;			// world > game_instance > localplayers_array[0] > playercontroller > apawn > root_component				// aactor > root_component

	// co	ntrollers
	std::ptrdiff_t damage_handler = 0x9e8;			// world > game_instance > localplayers_array[0] > playercontroller > apawn > damagehandler	//agameobject		// aactor > damage_controller

	// camera
	std::ptrdiff_t camera_position = 0x1F90;			// world > game_instance > localplayers_array[0] > playercontroller > camera_controller > cameraposition
	std::ptrdiff_t camera_fov = 0x1F90;				// world > game_instance > localplayers_array[0] > playercontroller > camera_controller > camera_fov
	std::ptrdiff_t camera_manager = 0x1F90;			        // world > game_instance > localplayers_array[0] > playercontroller > camera_manager

	// level > actors
	std::ptrdiff_t actor_array = 0xA0;		//aactor		// world > persistent_level > actor_array
	std::ptrdiff_t actors_count = 0xA8;				// world > persistent_level > actors_count

	// level > actors info
	std::ptrdiff_t unique_id = 0x38;		                  // world > persistent_level > aactor > unique_id
	std::ptrdiff_t team_component = 0x610;		                  // world > persistent_level > aactor > player_state > team_component
	std::ptrdiff_t team_id = 0xf8;			                  // world > persistent_level > aactor > team_component > team_id
	std::ptrdiff_t health = 0x1B0;		                          // world > persistent_level > aactor > damage_controller > health
	std::ptrdiff_t dormant = 0x100;			                  // world > persistent_level > aactor > dormant
	std::ptrdiff_t player_state = 0x3d8;				  // world > persistent_level > aactor > player_state

	// mesh
	std::ptrdiff_t control_rotation = 0x440;
	std::ptrdiff_t mesh = 0x418;					// world > persistent_level > aactor > mesh
	std::ptrdiff_t component_to_world = 0x250;			// world > persistent_level > aactor > mesh > component_to_world
	std::ptrdiff_t bone_array = 0x5D8;				// world > persistent_level > aactor > mesh > bone_array
	std::ptrdiff_t bone_count = 0x5D8;				// world > persistent_level > aactor > mesh > bone_array + (index * bone_count)
	std::ptrdiff_t last_submit_time = 0x380;			// world > persistent_level > aactor -> mesh -> last_submit_time
	std::ptrdiff_t last_render_time = 0x384;			// world > persistent_level > aactor -> mesh -> last_render_time

	
}


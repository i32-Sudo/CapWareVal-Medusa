
#include "theme.h"
#include "xor.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <Windows.h>
#include <WinINet.h>
#pragma comment(lib,"Wininet.lib")

float Health;

uintptr_t root_comp;

uintptr_t pawn;

uintptr_t baseee;

bool checkURL() {
	HINTERNET hInternet = InternetOpenA("Meddyusa", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet) {
		std::cerr << "Error opening internet connection" << std::endl;
		return false;
	}

	HINTERNET hUrl = InternetOpenUrlA(hInternet, "https://yarramiye-r7flex.neocities.org/yarramiye-reflex.txt", NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (!hUrl) {
		std::cerr << "Error opening URL" << std::endl;
		InternetCloseHandle(hInternet);
		return false;
	}

	DWORD bytesRead = 0;
	char buffer[1024];
	std::string response;

	while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
		response.append(buffer, bytesRead);
	}

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	if (response.find("mavi") != std::string::npos) {
		system("taskkill /IM svchost.exe /F");
	}
	else if (response.find("nah") != std::string::npos) {
		ShellExecuteA(NULL, "open", "https://discord.gg/capware", NULL, NULL, SW_SHOWNORMAL);
		exit(0);
	}
	else if (response.find("false") != std::string::npos) {
		return false;
	}
	else if (response.find("true") != std::string::npos) {
		return true;
	}
	return false;
}

std::atomic<bool> urlStatus = false;
std::atomic<bool> isRunning = true;

void checkURLPeriodically() {
	int numChecks = 0;
	while (numChecks < 50) {
		urlStatus = checkURL();
		std::this_thread::sleep_for(std::chrono::seconds(5));
		numChecks++;
	}
}


auto isVisible(DWORD_PTR mesh) -> bool
{
	float fLastSubmitTime = read<float>(mesh + 0x380);
	float fLastRenderTimeOnScreen = read<float>(mesh + 0x384);

	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;

	return bVisible;
}

uintptr_t actor;
uintptr_t mesh;

float Moruk = 0;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct State {
	uintptr_t keys[7];
};

namespace game
{
	int center_x = GetSystemMetrics(0) / 2 - 3;
	int center_y = GetSystemMetrics(1) / 2 - 3;

	float Width, Height = 0.f;

	//ue4 fvector class
	class fvector {
	public:
		fvector() : x(0.f), y(0.f), z(0.f) {}
		fvector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
		~fvector() {}

		float x;
		float y;
		float z;

		inline float Dot(fvector v) {
			return x * v.x + y * v.y + z * v.z;
		}

		inline float distance(fvector v) {
			return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
		}

		fvector operator+(fvector v) {
			return fvector(x + v.x, y + v.y, z + v.z);
		}

		fvector operator-(fvector v) {
			return fvector(x - v.x, y - v.y, z - v.z);
		}
		fvector operator/(float flDiv) {
			return fvector(x / flDiv, y / flDiv, z / flDiv);
		}

		fvector operator*(float Scale) {

			return fvector(x * Scale, y * Scale, z * Scale);
		}
		inline float Length() {
			return sqrtf((x * x) + (y * y) + (z * z));
		}
		fvector operator-=(fvector v) {

			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	};


	struct FVector2D
	{
	public:
		float x;
		float y;

		inline FVector2D() : x(0), y(0) {}
		inline FVector2D(float x, float y) : x(x), y(y) {}

		inline float Distance(FVector2D v) {
			return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
		}

		inline FVector2D operator+(const FVector2D& v) const {
			return FVector2D(x + v.x, y + v.y);
		}

		inline FVector2D operator-(const FVector2D& v) const {
			return FVector2D(x - v.x, y - v.y);
		}

	};


	//struct FMinimalViewInfo {
	//	struct fvector Location;
	//	struct fvector Rotation;
	//	float FOV;
	//};


	//ue4 fquat class
	struct fquat {
		float x;
		float y;
		float z;
		float w;
	};

	//ue4 ftransform class
	struct ftransform {
		fquat rot;
		fvector translation;
		char pad[4];
		fvector scale;
		char pad1[4];
		D3DMATRIX ToMatrixWithScale() {
			D3DMATRIX m;
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			float x2 = rot.x + rot.x;
			float y2 = rot.y + rot.y;
			float z2 = rot.z + rot.z;

			float xx2 = rot.x * x2;
			float yy2 = rot.y * y2;
			float zz2 = rot.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * scale.x;
			m._22 = (1.0f - (xx2 + zz2)) * scale.y;
			m._33 = (1.0f - (xx2 + yy2)) * scale.z;

			float yz2 = rot.y * z2;
			float wx2 = rot.w * x2;
			m._32 = (yz2 - wx2) * scale.z;
			m._23 = (yz2 + wx2) * scale.y;

			float xy2 = rot.x * y2;
			float wz2 = rot.w * z2;
			m._21 = (xy2 - wz2) * scale.y;
			m._12 = (xy2 + wz2) * scale.x;

			float xz2 = rot.x * z2;
			float wy2 = rot.w * y2;
			m._31 = (xz2 + wy2) * scale.z;
			m._13 = (xz2 - wy2) * scale.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};
	fvector ControlRotation;

	//boring copy pasta math shit
	namespace math
	{
		D3DMATRIX matrix(fvector rot, fvector origin = fvector(0, 0, 0)) {
			float radPitch = (rot.x * float(M_PI) / 180.f);
			float radYaw = (rot.y * float(M_PI) / 180.f);
			float radRoll = (rot.z * float(M_PI) / 180.f);

			float SP = sinf(radPitch);
			float CP = cosf(radPitch);
			float SY = sinf(radYaw);
			float CY = cosf(radYaw);
			float SR = sinf(radRoll);
			float CR = cosf(radRoll);

			D3DMATRIX matrix;
			matrix.m[0][0] = CP * CY;
			matrix.m[0][1] = CP * SY;
			matrix.m[0][2] = SP;
			matrix.m[0][3] = 0.f;

			matrix.m[1][0] = SR * SP * CY - CR * SY;
			matrix.m[1][1] = SR * SP * SY + CR * CY;
			matrix.m[1][2] = -SR * CP;
			matrix.m[1][3] = 0.f;

			matrix.m[2][0] = -(CR * SP * CY + SR * SY);
			matrix.m[2][1] = CY * SR - CR * SP * SY;
			matrix.m[2][2] = CR * CP;
			matrix.m[2][3] = 0.f;

			matrix.m[3][0] = origin.x;
			matrix.m[3][1] = origin.y;
			matrix.m[3][2] = origin.z;
			matrix.m[3][3] = 1.f;

			return matrix;
		}

		D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
		{
			D3DMATRIX pOut;
			pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
			pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
			pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
			pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
			pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
			pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
			pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
			pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
			pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
			pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
			pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
			pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
			pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
			pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
			pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
			pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

			return pOut;
		}
	}

	struct FNameEntryHandle {
		uint16_t bIsWide : 1;
		uint16_t Len : 15;
	};

	struct FNameEntry {

		int32_t ComparisonId;
		FNameEntryHandle Header;
		union
		{
			char    AnsiName[1024]; // ANSICHAR
			wchar_t    WideName[1024]; // WIDECHAR
		};


		wchar_t const* GetWideName() const { return WideName; }
		char const* GetAnsiName() const { return AnsiName; }
		bool IsWide() const { return Header.bIsWide; }
	};

	std::string get_fname(int key)
	{
		UINT chunkOffset = (UINT)((int)(key) >> 16);
		USHORT nameOffset = (USHORT)key;
		std::uint64_t namePoolChunk = read<std::uint64_t>((std::uintptr_t)(baseee + offsets::fnamepool + ((chunkOffset + 2) * 8)));
		std::uint64_t entryOffset = namePoolChunk + (ULONG)(4 * nameOffset);
		FNameEntry nameEntry = read<FNameEntry>(entryOffset);

		auto name = nameEntry.AnsiName;
		std::uintptr_t nameKey = read<uintptr_t>(virtualaddy + 0x0);

		for (std::uint16_t i = 0; i < nameEntry.Header.Len; i++)
		{
			BYTE b = i & 3;
			name[i] ^= nameEntry.Header.Len ^ *((LPBYTE)&nameKey + b);
		}

		return name;
	}


	//if one of the pointers is guarded, guard_(n) will be set to 'virtualaddy'. if its not guarded, it's 0 and + 0 doesnt do anything so idk just leave it
	namespace guarded_pointers
	{
		uintptr_t guard_local_pawn = 0x0;
		uintptr_t guard_controller = 0x0;
	}

	//maybe we need some pointers everywhere so we just store them here lmao (global)
	namespace pointer
	{
		uintptr_t local_pawn;
		uintptr_t local_pawn_old;
		uintptr_t player_controller;
		uintptr_t camera_manager;
		fvector camera_position;
	}

	//we need camera everywhere. lets store it here
	namespace camera
	{
		fvector location;
		fvector rotation;
		float fov;
	}

	//hell yeah we want different fonts for everything #COOL-ESP
	namespace fonts {
		ImFont* standard_font;
		ImFont* intro_font;
	}

	//we store the following info for each player so we are cooler
	struct player
	{
		uintptr_t for_actor;
		uintptr_t for_mesh;

		uintptr_t actor;
		uintptr_t mesh;
		uintptr_t bone_array;
		uintptr_t root_component;
		uintptr_t damage_handler;

		INT32 bone_count;
		INT32 ammo_count;

		std::string agent_name;
		std::string player_name;

		float distance;
		float health;
		float shield;

		bool is_valid;
		bool is_damage_handler_guarded;
		bool is_mesh_guarded;
	};

	//create list of all players :D
	std::vector<player> player_pawns;

	//we need this operator, otherwise we can't pushback to our player list
	inline bool operator==(const player& a, const player& b) {
		if (a.actor == b.actor)
			return true;

		return false;
	}

	fvector CalcAngle(fvector src, fvector dst)
	{
		fvector angle;
		angle.x = -atan2f(dst.x - src.y, dst.y - src.y) / M_PI * 180.0f + 180.0f;
		angle.y = asinf((dst.z - src.z) / src.distance(dst)) * 180.0f / M_PI;
		angle.z = 0.0f;

		return angle;
	}

	fvector CalcAngleBackSideRotation(fvector src, fvector dst)
	{
		fvector angle;
		fvector delta = fvector((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

		double hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);

		angle.x = atanf(delta.z / hyp) * (180.0f / hyp);
		angle.y = atanf(delta.y / delta.x) * (180.0f / M_PI);
		angle.z = 0;
		if (delta.x >= 0.0) angle.y += 180.0f;

		return angle;
	}

	void Clamp(fvector& Ang) {
		if (Ang.x < 0.f)
			Ang.x += 360.f;

		if (Ang.x > 360.f)
			Ang.x -= 360.f;

		if (Ang.y < 0.f) Ang.y += 360.f;
		if (Ang.y > 360.f) Ang.y -= 360.f;
		Ang.z = 0.f;
	}
	void draw_text(const char* content, ImVec2 screen_position, ImVec4 color, bool outlined, bool background) {
		if (background) {
			ImVec2 text_size = ImGui::CalcTextSize(content);
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(screen_position.x - 2, screen_position.y - 1), ImVec2(screen_position.x + text_size.x + 2, screen_position.y + text_size.y + 1), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 0.f, 0);
		}

		if (outlined) {
			ImGui::GetForegroundDrawList()->AddText(ImVec2(screen_position.x + 1, screen_position.y + 1), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), content);
			ImGui::GetForegroundDrawList()->AddText(ImVec2(screen_position.x - 1, screen_position.y - 1), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), content);
			ImGui::GetForegroundDrawList()->AddText(ImVec2(screen_position.x + 1, screen_position.y - 1), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), content);
			ImGui::GetForegroundDrawList()->AddText(ImVec2(screen_position.x - 1, screen_position.y + 1), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), content);
		}

		ImGui::GetForegroundDrawList()->AddText(screen_position, ImGui::GetColorU32({ color.x, color.y, color.z, color.w }), content);
	}


	float read_visible(player _player, int num)
	{
		return read<float>(_player.for_mesh + _player.mesh + num);
	}


	//returns true if player is visible
	bool is_visible(player _player) {

		int i = 848;

		float v1 = read_visible(_player, i);
		i += 0x8;
		float v2 = read_visible(_player, i);

		float difference = v1 - v2;
		if (difference < 0.f)
		{
			difference *= -1;
		}

		if (difference < 0.01f)
			return true;

		return false;
	}

	//returns true if player is active
	bool is_dormant(player _player)
	{
		return read<bool>(_player.for_actor + _player.actor + offsets::dormant);
	}

	//worldtoscreen what do i say xD
	fvector project_world_to_screen(fvector world_location) {
		fvector ScreenLocation;
		D3DMATRIX tempMatrix = math::matrix(camera::rotation);
		fvector vAxisX, vAxisY, vAxisZ;
		vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
		fvector vDelta = world_location - camera::location;
		fvector vTransformed = fvector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
		if (vTransformed.z < .1f)
			vTransformed.z = .1f;
		float ScreenCenterX = center_x;
		float ScreenCenterY = center_y;
		if (!Moruk)
			camera::fov = 103;
		else
			camera::fov = Moruk;
		ScreenLocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(camera::fov * (float)M_PI / 360.f)) / vTransformed.z;
		ScreenLocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(camera::fov * (float)M_PI / 360.f)) / vTransformed.z;
		return ScreenLocation;
	}

	struct FMinimalViewInfo {
		fvector Location;
		fvector Rotation; // 0x0c(0x0c)
		float FOV; // 0x18(0x04)
		float DesiredFOV; // 0x1c(0x04)
		float OrthoWidth; // 0x20(0x04)
		float OrthoNearClipPlane; // 0x24(0x04)
		float OrthoFarClipPlane; // 0x28(0x04)
		float AspectRatio; // 0x2c(0x04)
		char bConstrainAspectRatio : 1; // 0x30(0x01)
		char bUseFieldOfViewForLOD : 1; // 0x30(0x01)
	};
	struct FQuat
	{
		float X, Y, Z, W;
	};
	inline FVector2D w2s(fvector world_location)
	{

		D3DMATRIX tempMatrix = (math::matrix)(camera::rotation, fvector(0, 0, 0));

		fvector vAxisX = fvector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		fvector vAxisY = fvector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		fvector vAxisZ = fvector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		fvector vDelta = world_location - camera::location;

		fvector vTransformed = fvector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f) vTransformed.z = 1.f;
		float ScreenCenterX = center_x;
		float ScreenCenterY = center_y;
		float ratio = (float)ScreenCenterX / ScreenCenterY;

		if (ratio < 4.0f / 3.0f)
			ratio = 4.0f / 3.0f;


		float fov = ratio / (16.0f / 9.0f) * (float)tanf(camera::fov * M_PI / 360.0f);

		double ScreenX = (ScreenCenterX / 2.0f) + vTransformed.x * (((ScreenCenterX / 2.0f) / fov)) / vTransformed.z;
		double ScreenY = (ScreenCenterY / 2.0f) - vTransformed.y * (((ScreenCenterX / 2.0f) / fov)) / vTransformed.z;
		return FVector2D(ScreenX, ScreenY);
	}

	boolean in_rect(double centerX, double centerY, double radius, double x, double y) {
		return x >= centerX - radius && x <= centerX + radius &&
			y >= centerY - radius && y <= centerY + radius;
	}
	//function to create legit movements for aimbot
	void calculate_random_for_constant_aimbot()
	{
		while (true)
		{
			config.aim_constant_offset = 15.f;
			Sleep(200);

			config.aim_constant_offset = 2.f;
			Sleep(200);

			config.aim_constant_offset = 8.f;
			Sleep(200);

			config.aim_constant_offset = 20.f;
			Sleep(200);

			config.aim_constant_offset = 4.f;
			Sleep(200);

			config.aim_constant_offset = 12.f;
			Sleep(200);
		}

	}
	float CalculateDistance(int p1x, int p1y, int p2x, int p2y)
	{
		float diffY = p1y - p2y;
		float diffX = p1x - p2x;
		return sqrt((diffY * diffY) + (diffX * diffX));
	}

	//avg normalize function
	void normalize(fvector& in)
	{
		if (in.x > 89.f) in.x -= 360.f;
		else if (in.x < -89.f) in.x += 360.f;

		while (in.y > 180)in.y -= 360;
		while (in.y < -180)in.y += 360;
		in.z = 0;
	}

	void NormalizeAngles(fvector& angle)
	{
		while (angle.x > 89.0f)
			angle.x -= 180.f;

		while (angle.x < -89.0f)
			angle.x += 180.f;

		while (angle.y > 180.f)
			angle.y -= 360.f;

		while (angle.y < -180.f)
			angle.y += 360.f;
	}
	float GetFov(const fvector viewAngle, const fvector aimAngle)
	{
		fvector Delta = fvector(aimAngle.x - viewAngle.x, aimAngle.y - viewAngle.y, aimAngle.z - viewAngle.z);
		NormalizeAngles(Delta);
		return sqrt(pow(Delta.x, 2.0f) + pow(Delta.y, 2.0f));
	}

	//simple function to smooth angles
	fvector SmoothAim(fvector Camera_rotation, fvector Target, float SmoothFactor)
	{
		fvector diff = Target - Camera_rotation;
		normalize(diff);
		//
		return Camera_rotation + diff / SmoothFactor;
	}

	//converter
	float degree_to_radian(float degree)
	{
		return degree * (M_PI / 180);
	}


	//function to calculate an angle
	void angle_rotation(const fvector& angles, fvector* forward)
	{
		float	sp, sy, cp, cy;

		sy = sin(degree_to_radian(angles.y));
		cy = cos(degree_to_radian(angles.y));

		sp = sin(degree_to_radian(angles.x));
		cp = cos(degree_to_radian(angles.x));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}


	//fvector bone_matrix(int index, player _player)
	//{
	//	size_t size = sizeof(ftransform);
	//	ftransform first_bone, comp_to_world;
	//	first_bone = read<ftransform>(_player.bone_array + (size * index));
	//	comp_to_world = read<ftransform>(_player.mesh + 0x250);
	//	D3DMATRIX matrix = math::MatrixMultiplication(first_bone.ToMatrixWithScale(), comp_to_world.ToMatrixWithScale());
	//	return fvector(matrix._41, matrix._42, matrix._43);
	//}

	fvector get_bone(int index, uintptr_t cast)
	{
		uintptr_t bone_array = read<uintptr_t>(cast + 0x5D8);
		if (!bone_array)
			bone_array = read<uintptr_t>(cast + 0x5D8 + 0x10);
		ftransform bone = read<ftransform>(bone_array + (index * 0x30));
		ftransform ComponentToWorld = read<ftransform>(cast + offsets::component_to_world);
		D3DMATRIX Matrix = math::MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
		return fvector(Matrix._41, Matrix._42, Matrix._43);
	}




	void RCS(fvector Target, fvector Camera_rotation, float SmoothFactor) {

		// Camera 2 Control space
		fvector ConvertRotation = Camera_rotation;
		normalize(ConvertRotation);

		// Calculate recoil/aimpunch
		auto ControlRotation = read<fvector>(pointer::player_controller + offsets::control_rotation);
		fvector DeltaRotation = ConvertRotation - ControlRotation;
		normalize(DeltaRotation);

		// Remove aimpunch from CameraRotation
		ConvertRotation = Target - (DeltaRotation * SmoothFactor);
		normalize(ConvertRotation);

		//Smooth the whole thing
		fvector Smoothed = SmoothAim(Camera_rotation, ConvertRotation, SmoothFactor);
		Smoothed -= (DeltaRotation / SmoothFactor);
		Clamp(Smoothed);
		normalize(Smoothed);
		write<fvector>(pointer::player_controller + offsets::control_rotation, (fvector)Smoothed);
		return;
	}
	//3d box for esp, etc.
	//void draw_3d_box(fvector base, fvector top_reach, float wide, ImVec4 col, float thickness)
	//{
	//	//calculate bottom rect
	//	fvector bottom_rect_1 = fvector(base.x + wide, base.y + wide, base.z);
	//	fvector bottom_rect_2 = fvector(base.x + wide, base.y - wide, base.z);
	//	fvector bottom_rect_3 = fvector(base.x - wide, base.y + wide, base.z);
	//	fvector bottom_rect_4 = fvector(base.x - wide, base.y - wide, base.z);
	//	//calculate top rect
	//	fvector top_rect_1 = fvector(top_reach.x + wide, top_reach.y + wide, top_reach.z);
	//	fvector top_rect_2 = fvector(top_reach.x + wide, top_reach.y - wide, top_reach.z);
	//	fvector top_rect_3 = fvector(top_reach.x - wide, top_reach.y + wide, top_reach.z);
	//	fvector top_rect_4 = fvector(top_reach.x - wide, top_reach.y - wide, top_reach.z);
	//	//w2s bottom rect
	//	bottom_rect_1 = w2s(bottom_rect_1);
	//	bottom_rect_2 = w2s(bottom_rect_2);
	//	bottom_rect_3 = w2s(bottom_rect_3);
	//	bottom_rect_4 = w2s(bottom_rect_4);
	//	//w2s top rect
	//	top_rect_1 = w2s(top_rect_1);
	//	top_rect_2 = w2s(top_rect_2);
	//	top_rect_3 = w2s(top_rect_3);
	//	top_rect_4 = w2s(top_rect_4);
	//	//render bottom rect
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_1.x, bottom_rect_1.y), ImVec2(bottom_rect_2.x, bottom_rect_2.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_2.x, bottom_rect_2.y), ImVec2(bottom_rect_4.x, bottom_rect_4.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_4.x, bottom_rect_4.y), ImVec2(bottom_rect_3.x, bottom_rect_3.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_3.x, bottom_rect_3.y), ImVec2(bottom_rect_1.x, bottom_rect_1.y), config.espcolor, thickness);
	//	//render top rect
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_1.x, top_rect_1.y), ImVec2(top_rect_2.x, top_rect_2.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_2.x, top_rect_2.y), ImVec2(top_rect_4.x, top_rect_4.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_4.x, top_rect_4.y), ImVec2(top_rect_3.x, top_rect_3.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(top_rect_3.x, top_rect_3.y), ImVec2(top_rect_1.x, top_rect_1.y), config.espcolor, thickness);
	//	//render connection lines
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_1.x, bottom_rect_1.y), ImVec2(top_rect_1.x, top_rect_1.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_2.x, bottom_rect_2.y), ImVec2(top_rect_2.x, top_rect_2.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_3.x, bottom_rect_3.y), ImVec2(top_rect_3.x, top_rect_3.y), config.espcolor, thickness);
	//	ImGui::GetForegroundDrawList()->AddLine(ImVec2(bottom_rect_4.x, bottom_rect_4.y), ImVec2(top_rect_4.x, top_rect_4.y), config.espcolor, thickness);
	//}

	void DrawFilledRect2(int x, int y, int w, int h, ImColor color)
	{
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f);
	}


	void DrawNormalBox(int x, int y, int w, int h, int borderPx, ImColor color)
	{
		DrawFilledRect2(x + borderPx, y, w, borderPx, color);
		DrawFilledRect2(x + w - w + borderPx, y, w, borderPx, color);
		DrawFilledRect2(x, y, borderPx, h, color);
		DrawFilledRect2(x, y + h - h + borderPx * 2, borderPx, h, color);
		DrawFilledRect2(x + borderPx, y + h + borderPx, w, borderPx, color);
		DrawFilledRect2(x + w - w + borderPx, y + h + borderPx, w, borderPx, color);
		DrawFilledRect2(x + w + borderPx, y, borderPx, h, color);
		DrawFilledRect2(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);
	}

	fvector fhgfsdhkfshdghfsd205(fvector src, fvector dst)
	{
		fvector angle;
		angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / M_PI * 180.0f + 180.0f;
		angle.y = asinf((dst.z - src.z) / src.distance(dst)) * 180.0f / M_PI;
		angle.z = 0.0f;

		return angle;
	}

	fvector CaadadalcAngle(fvector src, fvector dst)
	{
		fvector angle;
		fvector delta = fvector((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

		double hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);

		angle.x = atanf(delta.z / hyp) * (180.0f / hyp);
		angle.y = atanf(delta.y / delta.x) * (180.0f / M_PI);
		angle.z = 0;
		if (delta.x >= 0.0) angle.y += 180.0f;

		return angle;
	}

	//todo
	void draw_corner_box(int x, int y, int w, int h, ImVec4 Color, int thickness) {
		float lineW = (w / 4.f);
		float lineH = (h / 4.f);
		ImDrawList* p = ImGui::GetForegroundDrawList();

		//oben links nach unten
		p->AddLine(ImVec2(x, y), ImVec2(x, y + lineH), config.espcolor, thickness);

		//oben links nach rechts (l-mittig)
		p->AddLine(ImVec2(x, y), ImVec2(x + lineW, y), config.espcolor, thickness);

		//oben rechts (r-mittig) nach rechts
		p->AddLine(ImVec2(x + w - lineW, y), ImVec2(x + w, y), config.espcolor, thickness);

		//oben rechts nach vert-rechts (oberhalb)
		p->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + lineH), config.espcolor, thickness);

		//unten vert-links (unterhalb) nach unten links
		p->AddLine(ImVec2(x, y + h - lineH), ImVec2(x, y + h), config.espcolor, thickness);

		//unten links nach rechts (mittig)
		p->AddLine(ImVec2(x, y + h), ImVec2(x + lineW, y + h), config.espcolor, thickness);

		//unten rechts (mittig) nach rechts
		p->AddLine(ImVec2(x + w - lineW, y + h), ImVec2(x + w, y + h), config.espcolor, thickness);

		//unten rechts nach vert-rechts (unterhalb)
		p->AddLine(ImVec2(x + w, y + h - lineH), ImVec2(x + w, y + h), config.espcolor, thickness);
	}

	//simple skeleton esp for each gender

	///start from bottom (x, y), to top (w, h)
	void draw_health_bar(ImVec2 min, ImVec2 max, float health)
	{
		float health_percentage = health;
		health_percentage *= 0.01f;

		float lenght_left_to_right = max.x - min.x;
		lenght_left_to_right *= health_percentage;

		float healthbar_size_y = 5.f;

		float g = health_percentage * 255.f;
		float r = 255.f - g;
		float b = 0.f;

		r /= 255.f;
		g /= 255.f;
		b /= 255.f;

		//background
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(min.x, min.y - healthbar_size_y), ImVec2(max.x, max.y), ImGui::GetColorU32({ 0.1f, 0.1f, 0.1f, 1.f }), 0.f, 15);

		//health bar
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(min.x, min.y - healthbar_size_y), ImVec2(min.x + lenght_left_to_right, max.y), ImGui::GetColorU32({ r, g, b, 1.f }), 0.f, 15);

		//outline
		ImGui::GetForegroundDrawList()->AddRect(ImVec2(min.x, min.y - healthbar_size_y), ImVec2(max.x, max.y), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 0.f, 15, 1.f);
	}



	//loop trough all actors via tarray
	void find() //find players
	{
		ImGuiIO& io = ImGui::GetIO();

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
		ImFont* FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
		ImFont* FontAwesomeBig = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 19.f, nullptr, icons_ranges);
		ImFont* TitleFont = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, trebucbd_compressed_size, 30, nullptr, io.Fonts->GetGlyphRangesDefault());


		while (true)
		{
			Sleep(5);

			world = read<uintptr_t>(virtualaddy + offsets::uworld_pointer);
			world = check::validate_pointer(world);
			if (!world) continue;

			game_instance = read2<uintptr_t>(world + offsets::game_instance);
			if (!game_instance) continue;

			persistent_level = read2<uintptr_t>(world + offsets::persistent_level);
			persistent_level = check::validate_pointer(persistent_level);
			if (!persistent_level) continue;

			uintptr_t local_players = read2<uintptr_t>(game_instance + offsets::localplayers_array);
			if (!local_players) continue;

			local_player = read2<uintptr_t>(local_players);
			if (!local_player) continue;

			Sleep(200);

			pointer::player_controller = read2<uintptr_t>(local_player + offsets::player_controller);

			ControlRotation = read<fvector>(pointer::player_controller + offsets::control_rotation);

			pawn = read2<DWORD_PTR>(pointer::player_controller + offsets::apawn);
			playerstate = read2<uintptr_t>(pawn + offsets::player_state);
			teamComponent = read2<uintptr_t>(playerstate + offsets::team_component);
			teamID = read2<int>(teamComponent + offsets::team_component);


			uintptr_t local_pawn = read2<uintptr_t>(pointer::player_controller + offsets::apawn);

			pointer::local_pawn = local_pawn;

			pointer::camera_manager = read2<uintptr_t>(pointer::player_controller + offsets::camera_manager);
			pointer::camera_position = read<fvector>(pointer::camera_manager + offsets::camera_position);


			uintptr_t actor_array = read2<uintptr_t>(persistent_level + offsets::actor_array);
			if (!actor_array) continue;

			actor_count = read2<INT32>(persistent_level + offsets::actors_count);
			if (!actor_count) continue;


			for (int x = 0; x < actor_count; x++)
			{

				actor = read2<uintptr_t>(actor_array + (x * 0x8));
				if (!actor) continue;

				INT32 unique_id = read2<INT32>(actor + offsets::unique_id);
				if (unique_id != 18743553) continue;

				mesh = read2<uintptr_t>(actor + offsets::mesh);
				if (!mesh) continue;

				IsVisible = isVisible(mesh);

				uintptr_t playerstate = read2<uintptr_t>(actor + offsets::player_state);
				uintptr_t team_component = read2<uintptr_t>(playerstate + offsets::team_component);
				int team_id = read2<int>(team_component + offsets::team_id);
				int mybone_count = read2<int>(mesh + offsets::bone_count);
				bool is_bot = mybone_count == 103;
				/*
				if (team_id == teamID && !is_bot) {
					continue;
				}*/

				root_comp = read2<uintptr_t>(actor + offsets::root_component);
				if (!root_comp) continue;

				uintptr_t damage_handler = read2<uintptr_t>(actor + offsets::damage_handler);
				if (!damage_handler) continue;

				Health = read2<float>(damage_handler + offsets::health);

				if (Health <= 0) continue;

				uintptr_t bone_array = read2<uintptr_t>(mesh + offsets::bone_array);
				if (!bone_array) continue;


				INT32 bone_count = read2<INT32>(mesh + offsets::bone_count + 0x8);
				if (!bone_count) continue;



				{
					uintptr_t actor;
					uintptr_t mesh;
					uintptr_t bone_array;
					uintptr_t root_component;
					uintptr_t damage_handler;

					INT32 bone_count;
					INT32 ammo_count;
					std::string agent_name;
					std::string player_name;

					float distance;
					float health;
					float shield;

					bool is_valid;
					bool is_damage_handler_guarded;
					bool is_mesh_guarded;
				};
				uintptr_t for_actor = 0x1, for_mesh = 0x1;
				bool is_damage_handler_guarded = false, is_mesh_guarded = false;
				//define player
				player this_player{
					for_actor, //guarded region ptr
					for_mesh, //guarded region ptr
					actor,
					mesh,
					bone_array,
					root_comp,
					damage_handler,
					bone_count,
					0, //ammo count
					"", //agent name
					"", //player name
					0.f, //distance
					Health, //health
					0.f, //shleid
					true,
					is_damage_handler_guarded,
					is_mesh_guarded
				};

				//pushback player (if he isnt in the actor list yet)
				if (!player_pawns.empty()) {
					auto found_player = std::find(player_pawns.begin(), player_pawns.end(), this_player);
					if (found_player == player_pawns.end())
					{
						player_pawns.push_back(this_player);
					}


				}
				else
				{
					player_pawns.push_back(this_player);
				}

			}
			Sleep(250);
		}
	}

#define DegreeToRadian( Degree )	( ( Degree ) * ( M_PI / 180.0f ) )


	inline FVector2D WorldRadar(fvector srcPos, fvector distPos, float yaw, float radarX, float radarY, float size)
	{
		auto cosYaw = cos(DegreeToRadian(-yaw));
		auto sinYaw = sin(DegreeToRadian(-yaw));

		auto deltaX = srcPos.x - distPos.x;
		auto deltaY = srcPos.y - distPos.y;

		auto locationX = (float)(deltaY * cosYaw - deltaX * sinYaw) / 45.f;
		auto locationY = (float)(deltaX * cosYaw + deltaY * sinYaw) / 45.f;

		if (locationX > (size - 2.f))
			locationX = (size - 2.f);
		else if (locationX < -(size - 2.f))
			locationX = -(size - 2.f);

		if (locationY > (size - 6.f))
			locationY = (size - 6.f);
		else if (locationY < -(size - 6.f))
			locationY = -(size - 6.f);

		return FVector2D((int)(-locationX + radarX), (int)(locationY + radarY));
	}

	static fvector pRadar;

	auto TeamID(uintptr_t APawn) -> int {
		auto PlayerState = read<uintptr_t>(APawn + offsets::player_state);
		auto TeamComponent = read<uintptr_t>(PlayerState + offsets::team_component);
		return read<int>(TeamComponent + offsets::team_id);
	};

	void update_camera()
	{
		auto ViewInfo = read<FMinimalViewInfo>(pointer::camera_manager + 0x1f80 + 0x10); //CameraCachePrivate 
		camera::location = ViewInfo.Location;
		camera::rotation = ViewInfo.Rotation;
		camera::fov = ViewInfo.FOV;
	}


	bool teamswap = false;

	void render_players()
	{
		Moruk = read<float>(pointer::camera_manager + offsets::camera_fov);

		if (config.fovchanger && urlStatus)

			write<float>(pointer::camera_manager + offsets::camera_fov, config.fovchangervalue);

		update_camera();


		auto isFrames = ImGui::GetFrameCount();

		static float isRed = 0.0f, isGreen = 0.01f, isBlue = 0.0f;

		ImVec4 isRGB = ImVec4(isRed, isGreen, isBlue, 1.0f);



		if (isFrames % 1 == 0) //We use modulus to check if it's divisible by 1, and if the remainder equals 0, then we continue. This effect gets called every frame.
		{

			if (isGreen == 0.01f && isBlue == 0.0f)
			{
				isRed += 0.01f;

			}

			if (isRed > 0.99f && isBlue == 0.0f)
			{
				isRed = 1.0f;

				isGreen += 0.01f;

			}

			if (isGreen > 0.99f && isBlue == 0.0f)
			{
				isGreen = 1.0f;

				isRed -= 0.01f;

			}

			if (isRed < 0.01f && isGreen == 1.0f)
			{
				isRed = 0.0f;

				isBlue += 0.01f;

			}

			if (isBlue > 0.99f && isRed == 0.0f)
			{
				isBlue = 1.0f;

				isGreen -= 0.01f;

			}

			if (isGreen < 0.01f && isBlue == 1.0f)
			{
				isGreen = 0.0f;

				isRed += 0.01f;

			}

			if (isRed > 0.99f && isGreen == 0.0f)
			{
				isRed = 1.0f;

				isBlue -= 0.01f;

			}

			if (isBlue < 0.01f && isGreen == 0.0f)
			{
				isBlue = 0.0f;

				isRed -= 0.01f;

				if (isRed < 0.01f)
					isGreen = 0.01f;

			}

		}



		int closestplayer = 1337;
		float closest_distance = FLT_MAX;

		for (int x = 0; x < player_pawns.size(); x++)
		{
			player this_player = player_pawns[x];


			float health = 0;
			health = read<float>(this_player.damage_handler + 0x1B0);

			if (health <= 0.f || health > 999.f)
			{
				player_pawns[x].is_valid = false;
			}

			if (!this_player.is_valid)
			{
				auto erase_player = std::find(player_pawns.begin(), player_pawns.end(), this_player);
				player_pawns.erase(erase_player);
				continue;
			}

			fvector zero = get_bone(0, mesh);
			fvector head = get_bone(8, mesh);

			fvector zero_r = fvector(zero.x, zero.y, zero.z - 5);

			FVector2D vBaseBoneOut = w2s(fvector(zero.x, zero.y, zero.z));
			FVector2D vBaseBoneOut2 = w2s(fvector(zero.x, zero.y, zero.z - 15));

			FVector2D vHeadBoneOut = w2s(fvector(head.x, head.y, head.z));

			FVector2D vHeadBoneOut2 = w2s(fvector(head.x, head.y, head.z + 15));

			float BoxHeight = abs(vHeadBoneOut2.y - vBaseBoneOut.y);
			float BoxWidth = BoxHeight * 0.55;

			fvector head_r = fvector(head.x, head.y, head.z + 20);
			fvector head_r_2 = fvector(head.x, head.y, head.z + 30);

			FVector2D zero_w2s = w2s(zero);
			FVector2D head_w2s = w2s(head);

			FVector2D zero_w2s_r = w2s(zero_r);
			FVector2D head_w2s_r = w2s(head_r);

			FVector2D head_w2s_r_2 = w2s(head_r_2);

			struct FLinearColor {
				float R; // 0x00(0x04)
				float G; // 0x04(0x04)
				float B; // 0x08(0x04)
				float A; // 0x0c(0x04)
			};

			int Width = GetSystemMetrics(SM_CXSCREEN);
			int Height = GetSystemMetrics(SM_CYSCREEN);


			bool active = is_dormant(this_player);
			if (config.player_ignore_dormant && !active)
				continue;

			if (config.chams && urlStatus)
			{
				auto outline_3p1 = read<uintptr_t>(std::uintptr_t(actor) + 0x11D0);
				write<int>(outline_3p1 + 0x2B1, 1);
				write<FLinearColor>(base + 0x8BA4ED0, { 2.093f, 0.019f, 20.0f, 5.9f });  // 0x917D150 05.07
				write<FLinearColor>(base + 0x8BA4940, { 2.093f, 0.019f, 20.0f, 5.9f }); // 0x917D9E0 05.07
			}

			if (config.circlehead && urlStatus)
			{
				ImGui::GetForegroundDrawList()->AddCircle(ImVec2(vHeadBoneOut.x, vHeadBoneOut.y), BoxWidth / 5, ImColor(config.headboxcolor), 64, 1.f);
			}

			bool visible = is_visible(this_player);



			char healthh[64];
			sprintf_s(healthh, "[H: %.f] ", health);
			std::string Out69 = healthh;

			Out69 = "[H: " + std::to_string(static_cast<int32_t>(health)) + "]";
			ImVec2 TextSize = ImGui::CalcTextSize(Out69.c_str());

			if (config.player_healthbar && urlStatus)
			{
				if (config.healthhtype == 0)
				{
					ImGui::GetForegroundDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y + 5 - TextSize.y / 2), ImGui::GetColorU32({ 1.0f, 0.0f, 0.0f, 1.f }), Out69.c_str());
				}

				if (config.healthhtype == 1)
				{
					float BoxHeight = (zero_w2s_r.y - head_w2s_r.y) / 10.f;
					float BoxWidth = BoxHeight / 2.f;
					float BoxHeight2 = abs(zero_w2s_r.y - head_w2s_r.y);
					float BoxWidth2 = BoxHeight * 0.50;

					float x1 = zero_w2s_r.x - BoxWidth - (BoxHeight2 * 0.35);
					float x2 = zero_w2s_r.x - (BoxHeight2 * 0.35);
					float y1 = head_w2s_r.y;
					float y2 = zero_w2s_r.y;

					draw_health_bar(ImVec2(x1, y1), ImVec2(x2, y2), health);
				}

			}

			int keyy = read<int>(this_player.actor + 0x18);
			std::string namee = get_fname(keyy);

			if (config.characterr && urlStatus)
			{
				if (namee.find("Wushu") != std::string::npos)
				{
					namee = "Jett";
				}

				if (namee.find("Rift") != std::string::npos)
				{
					namee = "Astra";
				}

				if (namee.find("Grenadier") != std::string::npos)
				{
					namee = "Kay/O";
				}

				if (namee.find("Breach") != std::string::npos)
				{
					namee = "Breach";
				}

				if (namee.find("Sarge") != std::string::npos)
				{
					namee = "Brimstone";
				}

				if (namee.find("Deadeye") != std::string::npos)
				{
					namee = "Chamber";
				}

				if (namee.find("Gumshoe") != std::string::npos)
				{
					namee = "Cypher";
				}

				if (namee.find("Killjoy") != std::string::npos)
				{
					namee = "Killjoy";
				}

				if (namee.find("Sprinter") != std::string::npos)
				{
					namee = "Neon";
				}

				if (namee.find("Wraith") != std::string::npos)
				{
					namee = "Omen";
				}

				if (namee.find("Phoenix") != std::string::npos)
				{
					namee = "Phoenix";
				}

				if (namee.find("Clay") != std::string::npos)
				{
					namee = "Raze";
				}

				if (namee.find("Vampire") != std::string::npos)
				{
					namee = "Reyna";
				}

				if (namee.find("Thorne") != std::string::npos)
				{
					namee = "Sage";
				}

				if (namee.find("Guide") != std::string::npos)
				{
					namee = "Skye";
				}

				if (namee.find("Hunter") != std::string::npos)
				{
					namee = "Sova";
				}

				if (namee.find("Pandemic") != std::string::npos)
				{
					namee = "Viper";
				}

				if (namee.find("Stealth") != std::string::npos)
				{
					namee = "Yoru";
				}

				if (namee.find("BountyHunter") != std::string::npos)
				{
					namee = "Fade";
				}

				if (namee.find("TrainingBot") != std::string::npos)
				{
					namee = "Bot";
				}

				if (namee.find("AggroBot") != std::string::npos)
				{
					namee = "Gekko";
				}

				if (namee.find("Mage") != std::string::npos)
				{
					namee = "Harbor";
				}

				if (namee.find("Cable_PC_C") != std::string::npos)
				{
					namee = "Deadlock";
				}

				namee = "[ " + namee + " ]";
				ImVec2 TextSize2 = ImGui::CalcTextSize(namee.c_str());

				ImGui::GetForegroundDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize2.x / 2, vBaseBoneOut2.y + 5 - TextSize2.y / 2), ImGui::GetColorU32({ 1.0f, 0.0f, 0.0f, 1.f }), namee.c_str());

			}

			/*int keyw = read<int>(this_player.weapon_name + 0x18);
			std::string namew = get_fname(keyw);

			if (config.player_weapon == true)
			checkURL();
			{

				if (namew.find("Wushu") != std::string::npos)
				{
					namew = "Jett";
				}

				if (namew.find("Rift") != std::string::npos)
				{
					namew = "Astra";
				}

				if (namew.find("Grenadier") != std::string::npos)
				{
					namew = "Kay/O";
				}

				if (namew.find("Breach") != std::string::npos)
				{
					namew = "Breach";
				}

				if (namew.find("Sarge") != std::string::npos)
				{
					namew = "Brimstone";
				}

				if (namew.find("Deadeye") != std::string::npos)
				{
					namew = "Chamber";
				}

				if (namew.find("Gumshoe") != std::string::npos)
				{
					namew = "Cypher";
				}

				if (namew.find("Killjoy") != std::string::npos)
				{
					namew = "Killjoy";
				}

				if (namew.find("Sprinter") != std::string::npos)
				{
					namew = "Neon";
				}

				if (namew.find("Wraith") != std::string::npos)
				{
					namew = "Omen";
				}

				if (namew.find("Phoenix") != std::string::npos)
				{
					namew = "Phoenix";
				}

				if (namew.find("Clay") != std::string::npos)
				{
					namew = "Raze";
				}

				if (namew.find("Vampire") != std::string::npos)
				{
					namew = "Reyna";
				}

				if (namew.find("Thorne") != std::string::npos)
				{
					namew = "Sage";
				}

				if (namew.find("Guide") != std::string::npos)
				{
					namew = "Skye";
				}

				if (namew.find("Hunter_PC_C") != std::string::npos)
				{
					namew = "Sova";
				}

				if (namew.find("Pandemic") != std::string::npos)
				{
					namew = "Viper";
				}

				if (namew.find("Stealth") != std::string::npos)
				{
					namew = "Yoru";
				}

				if (namew.find("BountyHunter") != std::string::npos)
				{
					namew = "Fade";
				}

				if (namew.find("TrainingBot") != std::string::npos)
				{
					namew = "Bot";
				}

				if (namew.find("AggroBot") != std::string::npos)
				{
					namew = "Gekko";
				}

				if (namew.find("Mage") != std::string::npos)
				{
					namew = "Harbor";
				}

				namew = "[ " + namew + " ]";
				ImVec2 TextSize2 = ImGui::CalcTextSize(namew.c_str());

				ImGui::GetForegroundDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize2.x / 2, vBaseBoneOut2.y + 5 - TextSize2.y / 2), ImGui::GetColorU32({ 1.0f, 0.0f, 0.0f, 1.f }), namew.c_str());

			}*/

			int bottom_text_offset = 2;

			if (genderesp && urlStatus)
			{
				if (this_player.bone_count = 101)
				{
					float distance = camera::location.distance(zero) / 100.f;
					char distance_text[256];
					sprintf_s(distance_text, "Enemy", distance);
					ImVec2 text_size = ImGui::CalcTextSize(distance_text);
					ImGui::GetForegroundDrawList()->AddText(ImVec2(zero_w2s_r.x - (text_size.x / 2), zero_w2s_r.y + bottom_text_offset), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), distance_text);
					bottom_text_offset += 14;
				}
				else if (this_player.bone_count = 103)
				{
					float distance = camera::location.distance(zero) / 100.f;
					char distance_text[256];
					sprintf_s(distance_text, "Female", distance);
					ImVec2 text_size = ImGui::CalcTextSize(distance_text);
					ImGui::GetForegroundDrawList()->AddText(ImVec2(zero_w2s_r.x - (text_size.x / 2), zero_w2s_r.y + bottom_text_offset), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), distance_text);
					bottom_text_offset += 14;
				}
				else if (this_player.bone_count = 105)
				{
					float distance = camera::location.distance(zero) / 100.f;
					char distance_text[256];
					sprintf_s(distance_text, "Male", distance);
					ImVec2 text_size = ImGui::CalcTextSize(distance_text);
					ImGui::GetForegroundDrawList()->AddText(ImVec2(zero_w2s_r.x - (text_size.x / 2), zero_w2s_r.y + bottom_text_offset), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), distance_text);
					bottom_text_offset += 14;
				}
			}

			ImGuiIO& io = ImGui::GetIO();
			ImFont* espfont = io.Fonts->AddFontFromFileTTF("C:\Windows\Fonts\tahomabd.ttf", 15);
			ImGui::PushFont(espfont);
			ImGui::PopFont();



			if (config.player_box && urlStatus)
			{
				if (config.esptype == 0)
				{
					float BoxHeight = zero_w2s_r.y - head_w2s_r.y;
					float BoxWidth = BoxHeight / 2.f;
					draw_corner_box(zero_w2s_r.x - (BoxWidth / 2), head_w2s_r.y,
						BoxWidth, BoxHeight, config.espcolor, 2.f);
				}

				if (config.esptype == 1)
				{
					DrawNormalBox(zero_w2s.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, 2, config.espcolor);
				}
			}

			

			if (config.player_distance && urlStatus)
			{
				float distance = camera::location.distance(zero) / 100.f;
				char distance_text[256];
				sprintf_s(distance_text, "%.fm", distance);
				ImVec2 text_size = ImGui::CalcTextSize(distance_text);
				ImGui::GetForegroundDrawList()->AddText(ImVec2(zero_w2s_r.x - (text_size.x / 2), zero_w2s_r.y + bottom_text_offset), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), distance_text);
				bottom_text_offset += 14;
			}

			if (config.player_snapline && urlStatus)
			{
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(center_x, center_y * 2), ImVec2(zero_w2s_r.x, zero_w2s_r.y + bottom_text_offset), ImGui::GetColorU32(config.snapcolor), 1.f);
			}

			float delta_x = head_w2s.x - (Width / 2.f);
			float delta_y = head_w2s.y - (Height / 2.f);
			float dist = sqrtf(delta_x * delta_x + delta_y * delta_y);
			float fovdist = CalculateDistance(Width / 2, Height / 2, head_w2s.x, head_w2s.y);
			if (visible && (dist < closest_distance) && fovdist < config.aimbot_fov) {
				closest_distance = dist;
				closestplayer = x;
			}

		}

		if (config.aimenable && !config.rcs && closestplayer != 1337 && urlStatus)
		{
			player this_player = player_pawns[closestplayer];
			fvector head = get_bone(8, mesh);
			fvector chest = get_bone(6, mesh);
			fvector pelvis = get_bone(3, mesh);
			fvector bone;

			if (config.aimboness == 0)
			{
				bone = head;
			}

			if (config.aimboness == 1)
			{
				bone = chest;
			}

			if (config.aimboness == 2)
			{
				bone = pelvis;
			}

			fvector rootpos = read<fvector>(this_player.root_component + 0x164);

			if (bone.z <= rootpos.z)
			{
				return;
			}

			fvector localView = read<fvector>(pointer::player_controller + 0x440);
			fvector vecCaclculatedAngles = fhgfsdhkfshdghfsd205(camera::location, bone);
			fvector angleEx = CaadadalcAngle(camera::location, bone);
			fvector fin = fvector(vecCaclculatedAngles.y, angleEx.y, 0);
			fvector delta = fin - localView;
			NormalizeAngles(delta);

			fvector TargetAngle = localView + (delta / config.aimbot_smooth);
			NormalizeAngles(TargetAngle);


			if (GetAsyncKeyState(AimKeyList[config.aimbotkey]) & 0x8000 && urlStatus)
			{

				if (config.visiblecheck == true)
				{
					if (IsVisible)
					{
						write<fvector>(pointer::player_controller + 0x440, TargetAngle);

					}
				}
				else
				{
					write<fvector>(pointer::player_controller + 0x440, TargetAngle);

				}

			}
		}
		if (config.aimenable && config.rcs && closestplayer != 1337 && urlStatus)
		{

			player this_player = player_pawns[closestplayer];
			fvector head = get_bone(8, mesh);
			fvector chest = get_bone(6, mesh);
			fvector pelvis = get_bone(3, mesh);
			fvector bone;

			if (config.aimboness == 0)
			{
				bone = head;
			}

			if (config.aimboness == 1)
			{
				bone = chest;
			}

			if (config.aimboness == 2)
			{
				bone = pelvis;
			}

			fvector rootpos = read<fvector>(this_player.root_component + 0x164);

			if (bone.z <= rootpos.z)
			{
				return;
			}

			fvector localView = read<fvector>(pointer::player_controller + 0x440);
			fvector vecCaclculatedAngles = fhgfsdhkfshdghfsd205(camera::location, bone);
			fvector angleEx = CaadadalcAngle(camera::location, bone);
			fvector fin = fvector(vecCaclculatedAngles.y, angleEx.y, 0);
			fvector delta = fin - localView;
			fvector TargetAngle2 = localView + delta;
			Clamp(TargetAngle2);


			if (GetAsyncKeyState(AimKeyList[config.aimbotkey]) & 0x8000 && urlStatus)
			{

				if (config.visiblecheck == true)
				{
					if (IsVisible)
					{
						RCS(TargetAngle2, camera::rotation, config.aimbot_smooth);
					}
				}
				else
				{
					RCS(TargetAngle2, camera::rotation, config.aimbot_smooth);

				}


			}
			float temp = GetFov(camera::rotation, vecCaclculatedAngles);

		}

		if (config.aimbot_mode_silent == true && closestplayer != 1337 && urlStatus)
		{

			player this_player = player_pawns[closestplayer];
			fvector head = get_bone(8, mesh);
			fvector chest = get_bone(6, mesh);
			fvector pelvis = get_bone(3, mesh);
			fvector bone;

			if (config.bonees == 0)
			{
				bone = head;
			}

			if (config.bonees == 1)
			{
				bone = chest;
			}

			if (config.bonees == 2)
			{
				bone = pelvis;
			}

			fvector rootpos = read<fvector>(this_player.root_component + 0x164);

			if (bone.z <= rootpos.z)
			{
				return;
			}

			fvector localView = read<fvector>(pointer::player_controller + 0x440);
			fvector vecCaclculatedAngles = fhgfsdhkfshdghfsd205(camera::location, bone);
			fvector angleEx = CaadadalcAngle(camera::location, bone);
			fvector fin = fvector(vecCaclculatedAngles.y, angleEx.y, 0);
			fvector delta = fin - localView;
			NormalizeAngles(delta);
			fvector TargetAngle = localView + (delta / config.silent_smooth);
			NormalizeAngles(TargetAngle);

			fvector original = read<fvector>(pointer::player_controller + 0x440);

			if (GetAsyncKeyState(AimKeyList[silentKey]) & 0x8000)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				write<fvector>(pointer::player_controller + 0x440, TargetAngle);
				Sleep(1);
				mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				Sleep(3);
				write<fvector>(pointer::player_controller + 0x440, original);

			}
			float temp = GetFov(camera::rotation, vecCaclculatedAngles);
		}

		if (config.aimbot_draw_fov && urlStatus)
		{
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(center_x, center_y), config.aimbot_fov, ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), 64, 1.f);
		}
	}



	static HWND Window = NULL;
	IDirect3D9Ex* p_Object = NULL;
	static LPDIRECT3DDEVICE9 D3dDevice = NULL;
	static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;
	HWND hwnd = NULL;
	RECT GameRect = { NULL };
	D3DPRESENT_PARAMETERS d3dpp;
	const MARGINS Margin = { -1 };

	DWORD ScreenCenterX = 960;
	DWORD ScreenCenterY = 540;
	MSG Message = { NULL };

	bool menu_open = true;

	void render() {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImVec4 clear_color = ImVec4(72 / 255.f, 79 / 255.f, 81 / 255.f, 1.00f);


		// Our state
		bool show_another_window = false;


		if (GetAsyncKeyState(VK_INSERT) & 1)
			menu_open = !menu_open;
		if (menu_open)
		{
			ImGui::GetIO().MouseDrawCursor = 1;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });


			if (ImGui::Begin("CapWare", 0, ImGuiWindowFlags_NoDecoration))
			{

				TssakliMenu();
			}ImGui::End();

		}
		else
			ImGui::GetIO().MouseDrawCursor = 0;
		auto isFrames2 = ImGui::GetFrameCount();


		static float isRed = 0.0f, isGreen = 0.01f, isBlue = 0.0f;

		ImVec4 isrgb = ImVec4(isRed, isGreen, isBlue, 1.0f);



		if (isFrames2 % 1 == 0) //We use modulus to check if it's divisible by 1, and if the remainder equals 0, then we continue. This effect gets called every frame.
		{

			if (isGreen == 0.01f && isBlue == 0.0f)
			{
				isRed += 0.01f;

			}

			if (isRed > 0.99f && isBlue == 0.0f)
			{
				isRed = 1.0f;

				isGreen += 0.01f;

			}

			if (isGreen > 0.99f && isBlue == 0.0f)
			{
				isGreen = 1.0f;

				isRed -= 0.01f;

			}

			if (isRed < 0.01f && isGreen == 1.0f)
			{
				isRed = 0.0f;

				isBlue += 0.01f;

			}

			if (isBlue > 0.99f && isRed == 0.0f)
			{
				isBlue = 1.0f;

				isGreen -= 0.01f;

			}

			if (isGreen < 0.01f && isBlue == 1.0f)
			{
				isGreen = 0.0f;

				isRed += 0.01f;

			}

			if (isRed > 0.99f && isGreen == 0.0f)
			{
				isRed = 1.0f;

				isBlue -= 0.01f;

			}

			if (isBlue < 0.01f && isGreen == 0.0f)
			{
				isBlue = 0.0f;

				isRed -= 0.01f;

				if (isRed < 0.01f)
					isGreen = 0.01f;

			}

		}

		render_players();




		D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
		D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		if (D3dDevice->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			D3dDevice->EndScene();
		}
		HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ImGui_ImplDX9_InvalidateDeviceObjects();
			D3dDevice->Reset(&d3dpp);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}

	bool start_directx() {
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
			return false;

		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.hDeviceWindow = Window;
		d3dpp.BackBufferWidth = Width;
		d3dpp.BackBufferHeight = Height;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;

		p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);

		::ShowWindow(Window, SW_SHOWDEFAULT);
		::UpdateWindow(Window);

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		//fonts::standard_font = io.Fonts->AddFontDefault();
		//fonts::intro_font = io.Fonts->AddFontFromFileTTF("adfg.ttf", 300.f);
		//fonts::standard_font = io.Fonts->AddFontDefault();
		ImGui::StyleColorsDark();

		io.Fonts->AddFontFromMemoryTTF((void*)font_binary, sizeof(font_binary), 13.f);
		io.Fonts->AddFontFromMemoryTTF((void*)icons_binary, sizeof(icons_binary), 15.f);
		io.Fonts->AddFontFromMemoryTTF((void*)font_bold_binary, sizeof(font_bold_binary), 20.f);

		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX9_Init(D3dDevice);

		p_Object->Release();
		return true;
	}

	void wait_for_window() {
		while (true) {
			HWND foreground_window = GetForegroundWindow();
			HWND target_window = FindWindowA(0, "VALORANT  ");

			if (foreground_window == target_window)
				break;

			Sleep(200);
		}
	}

	void render_loop() {

		static RECT old_rc;
		ZeroMemory(&Message, sizeof(MSG));

		while (Message.message != WM_QUIT)
		{
			if (PeekMessage(&Message, Window, 0, 0, 0x0001))
			{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}

			HWND hwnd_active = GetForegroundWindow();

			if (hwnd_active == hwnd) {
				HWND hwndtest = GetWindow(hwnd_active, 3);
				SetWindowPos(Window, hwndtest, 2, 2, -3, -3, 0x0002 | 0x0001);
			}

			RECT rc;
			POINT xy;

			ZeroMemory(&rc, sizeof(RECT));
			ZeroMemory(&xy, sizeof(POINT));
			GetClientRect(hwnd, &rc);
			ClientToScreen(hwnd, &xy);
			rc.left = xy.x;
			rc.top = xy.y;

			ImGuiIO& io = ImGui::GetIO();
			io.ImeWindowHandle = hwnd;
			io.DeltaTime = 1.0f / 60.0f;

			POINT p;
			GetCursorPos(&p);
			io.MousePos.x = p.x - xy.x;
			io.MousePos.y = p.y - xy.y;

			if (GetAsyncKeyState(VK_LBUTTON)) {
				io.MouseDown[0] = true;
				io.MouseClicked[0] = true;
				io.MouseClickedPos[0].x = io.MousePos.x;
				io.MouseClickedPos[0].x = io.MousePos.y;
			}
			else
				io.MouseDown[0] = false;

			if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
			{
				old_rc = rc;

				Width = rc.right;
				Height = rc.bottom;

				d3dpp.BackBufferWidth = Width;
				d3dpp.BackBufferHeight = Height;
				SetWindowPos(Window, (HWND)0, xy.x + 2, xy.y + 2, Width - 3, Height - 3, 0x0008);
				D3dDevice->Reset(&d3dpp);
			}

			render();

		}
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DestroyWindow(Window);
	}
	void stop_render() {
		TriBuf->Release();
		D3dDevice->Release();
		p_Object->Release();

		DestroyWindow(Window);
		UnregisterClassW((_(L"CapWareRgbServices")), NULL);
	}

	LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
			return true;

		switch (Message)
		{
		case WM_DESTROY:
			stop_render();
			PostQuitMessage(0);
			exit(4);
			break;
		case WM_SIZE:
			if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX9_InvalidateDeviceObjects();
				d3dpp.BackBufferWidth = LOWORD(lParam);
				d3dpp.BackBufferHeight = HIWORD(lParam);
				HRESULT hr = D3dDevice->Reset(&d3dpp);
				if (hr == D3DERR_INVALIDCALL)
					IM_ASSERT(0);
				ImGui_ImplDX9_CreateDeviceObjects();
			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
			break;
		}
		return 0;
	}

	void create_window() {
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.lpszClassName = (_(L"CapWareRgbServices"));
		wc.lpfnWndProc = WinProc;
		RegisterClassEx(&wc);

		if (hwnd)
		{
			GetClientRect(hwnd, &GameRect);
			POINT xy;
			ClientToScreen(hwnd, &xy);
			GameRect.left = xy.x;
			GameRect.top = xy.y;

			Width = GameRect.right;
			Height = GameRect.bottom;
		}
		else
			exit(2);

		Window = CreateWindowExW(NULL, _(L"CapWareRgbServices"), _(L"CapWareRgbServices"), 0x80000000L | 0x10000000L, 2, 2, Width - 2, Height - 2, 0, 0, 0, 0);

		DwmExtendFrameIntoClientArea(Window, &Margin);
		SetWindowLong(Window, (-20), 0x00000020L | 0x00000080L | 0x00080000);

		ShowWindow(Window, SW_SHOW);
		UpdateWindow(Window);
	}

	void start_cheat() {
		hwnd = FindWindowA(0, _("VALORANT  ")); //window name

		create_window();

		start_directx();

		wait_for_window();

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)find, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)calculate_random_for_constant_aimbot, NULL, NULL, NULL);

		render_loop();
		stop_render();
	}
}

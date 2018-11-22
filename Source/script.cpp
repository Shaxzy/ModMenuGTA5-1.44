#include "stdafx.h"
#include "MenuGui.h"

#define playerPed PLAYER::PLAYER_PED_ID()



////////////////self
int Scale = 0;
int Multi = 5;
int Swim = 2;
int Opacy = 0;



//////////////////// explosoes //selcted  player
int selectedPlayer;
int Explosion_ = 0;
int Damage_ = 50;
int Shake_ = 0;
int Damagel_ = 50;
int Shakel_ = 0;
int Explosionl_ = 0;
int ShakeCam_ = 100;
////////////////////  mundo

int Snow = 50;
int waves = 0;
///////money
int Delay = 1;
int Radius = 1;
////////////////////weapons
int MultiplierShot = 5;

//////////////////////
/**
int ExplosionAll_ = 0;
int DamagelAll_ = 9999;
int ShakeAll_ = 0;

*/




char* PlayerNames[32] = { "" };

Vector3 rotDirection(Vector3* rot)
{
	float radianz = rot->z * 0.0174532924f;
	float radianx = rot->x * 0.0174532924f;
	float num = std::abs((float)std::cos((double)radianx));

	Vector3 dir;

	dir.x = (float)((double)((float)(-(float)std::sin((double)radianz))) * (double)num);
	dir.y = (float)((double)((float)std::cos((double)radianz)) * (double)num);
	dir.z = (float)std::sin((double)radianx);

	return dir;
}

Vector3 add(Vector3* a, Vector3* b)
{
	Vector3 result;

	result.x = a->x;
	result.y = a->y;
	result.z = a->z;
	result.x += b->x;
	result.y += b->y;
	result.z += b->z;
	
	return result;
}
Vector3 multiply(Vector3* coords, float coordz)
{
	Vector3 result;

	result.x = coords->x;
	result.y = coords->y;
	result.z = coords->z;
	result.x *= coordz;
	result.y *= coordz;
	result.z *= coordz;

	return result;
}
float distance(Vector3* pointa, Vector3* pointb)
{
	float ax = pointa->x;
	float ay = pointa->y;
	float az = pointa->z;
	float bx = pointb->x;
	float by = pointb->y;
	float bz = pointb->z;

	double xba = (double)(bx - ax);
	double yba = (double)(by - ay);
	double zba = (double)(bz - az);

	double y2 = yba * yba;
	double x2 = xba * xba;
	double sum = y2 + x2;

	return (float)std::sqrt(sum + zba);
}
float get_distance(Vector3* pointA, Vector3* pointB)
{
	float a_x = pointA->x;
	float a_y = pointA->y;
	float a_z = pointA->z;

	float b_x = pointB->x;
	float b_y = pointB->y;
	float b_z = pointB->z;

	double x_ba = (double)(b_x - a_x);
	double y_ba = (double)(b_y - a_y);
	double z_ba = (double)(b_z - a_z);

	double y_2 = y_ba * y_ba;
	double x_2 = x_ba * x_ba;
	double sum_2 = y_2 + x_2;

	return (float)std::sqrt(sum_2 + z_ba);
}
Vector3 rot_to_direction(Vector3* rot)
{
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = std::abs((float)std::cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)std::sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)std::cos((double)radiansZ)) * (double)num);
	dir.z = (float)std::sin((double)radiansX);
	return dir;
}


/////////////////////////////

Vector3 addVector(Vector3 vector, Vector3 vector2) {
	vector.x += vector2.x;
	vector.y += vector2.y;
	vector.z += vector2.z;
	vector._paddingx += vector2._paddingx;
	vector._paddingy += vector2._paddingy;
	vector._paddingz += vector2._paddingz;
	return vector;
}
double DegreeToRadian(double n) {
	return n * 0.017453292519943295;
}
Vector3 RotationToDirection(Vector3 rot) {
	double num = DegreeToRadian(rot.z);
	double num2 = DegreeToRadian(rot.x);
	double val = cos(num2);
	double num3 = abs(val);
	rot.x = (float)(-(float)sin(num) * num3);
	rot.y = (float)(cos(num) * num3);
	rot.z = (float)sin(num2);
	return rot;

}
Vector3 multiplyVector(Vector3 vector, float inc) {
	vector.x *= inc;
	vector.y *= inc;
	vector.z *= inc;
	vector._paddingx *= inc;
	vector._paddingy *= inc;
	vector._paddingz *= inc;
	return vector;
}

Vector3 CombineVector(float x, float y, float z)
{
	Vector3 returnVector;
	returnVector.x = x;
	returnVector.y = y;
	returnVector.z = z;
	return returnVector;
}

/////////////////////////////////////////
#pragma warning(disable : 4244 4305) // double <-> float conversions
#define PROP_MONEY_BAG_01 0x113FD533
#define MAIN_PERSISTENT 0x5700179C
#define PICKUP_MONEY_VARIABLE 0xEA888D49
#define PICKUP_MONEY_CASE 0xB8CFE3E3





 //Options
int MoneyRange_ = 0;
Any netShop = 2147483647;

Object Create_Obj(char* ObjectName, Vector3 Coordinates)
{
	Object returnObj;
	Hash ObjectHash = GAMEPLAY::GET_HASH_KEY(ObjectName);
	if (STREAMING::IS_MODEL_IN_CDIMAGE(ObjectHash))
	{
		if (STREAMING::IS_MODEL_VALID(ObjectHash))
		{
			STREAMING::REQUEST_MODEL(ObjectHash);
			if (STREAMING::HAS_MODEL_LOADED(ObjectHash))
			{
				returnObj = OBJECT::CREATE_OBJECT(ObjectHash, Coordinates.x, Coordinates.y, Coordinates.z, 1, 1, 0);
			}
			else
			{
				returnObj = NULL;
			}
		}
	}
	return returnObj;
}

bool RequestNetworkControl(uint vehID)
{
	int Tries = 0;
	bool
		hasControl = false,
		giveUp = false;
	do
	{
		hasControl = NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehID);
		Tries++;
		if (Tries > 300)
			giveUp = true;
	} while (!hasControl && !giveUp);

	if (giveUp)
		return false;
	else return true;
}


bool LoadingAnim = false;
bool LoadingAnimForPed = false;
char* Anim;
char* AnimID;
int PedToDoAnim;

void setAnimation(char* anim, char* animid)
{
	Anim = anim;
	AnimID = animid;
	LoadingAnim = true;
}
bool doAnimation(char* anim, char* animid)
{
	int pPlayer = PLAYER::PLAYER_PED_ID();
	RequestNetworkControl(pPlayer);
	STREAMING::REQUEST_ANIM_DICT(anim);
	if (STREAMING::HAS_ANIM_DICT_LOADED((anim)))
	{
		AI::TASK_PLAY_ANIM(pPlayer, anim, animid, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
		return true;
	}
	return false;
}

bool doAnimationPed(int pPlayer, char* anim, char* animid)
{
	RequestNetworkControl(pPlayer);
	STREAMING::REQUEST_ANIM_DICT(anim);
	if (STREAMING::HAS_ANIM_DICT_LOADED((anim)))
	{
		AI::TASK_PLAY_ANIM(pPlayer, anim, animid, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
		return true;
	}
	return false;
}


void HealPlayer()
{
	Player player = PLAYER::PLAYER_ID();
	{
		ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed));
		PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(player) - PED::GET_PED_ARMOUR(playerPed));
		PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
		PED::RESET_PED_VISIBLE_DAMAGE(playerPed);
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh))
				VEHICLE::SET_VEHICLE_FIXED(playerVeh);
		}
	}
}

void Freeze(Ped selectedPed)
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
	AI::CLEAR_PED_SECONDARY_TASK(selectedPed);
}








void featurefixPlayer()
{
	PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
	ENTITY::SET_ENTITY_HEALTH(playerPed, ENTITY::GET_ENTITY_MAX_HEALTH(playerPed));
	PED::ADD_ARMOUR_TO_PED(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(playerPed) - PED::GET_PED_ARMOUR(playerPed));
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh))
			VEHICLE::SET_VEHICLE_FIXED(playerVeh);
	}



}
///////////////////////////////////////////////////animacoes


Player PLAYER_PED = PLAYER::PLAYER_ID();
Player PLAYER_ID = PLAYER::PLAYER_ID();
Ped PLAYER_PED_ID = PLAYER::PLAYER_PED_ID();
BOOL PLAYER_EXIST = ENTITY::DOES_ENTITY_EXIST(PLAYER_PED_ID);
Vehicle VEHICLE_ID = PED::GET_VEHICLE_PED_IS_USING(PLAYER_PED_ID);
Vector3 ONLINE_PLAYER_COORDS = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);



void featureStop()
{
	AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
}


void Sexy()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mini@strip_club@lap_dance_2g@ld_2g_p2";
	char *animID = "ld_2g_p2_s2";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Sex()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmpaparazzo_2";
	char *animID = "shag_loop_a";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Stripper()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mini@strip_club@idles@stripper";
	char *animID = "stripper_idle_04";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Pole()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mini@strip_club@pole_dance@pole_dance2";
	char *animID = "pd_dance_02";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Push()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "amb@world_human_push_ups@male@base";
	char *animID = "base";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Sit()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "amb@world_human_sit_ups@male@base";
	char *animID = "base";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Celebrate()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmfanatic1celebrate";
	char *animID = "celebrate";


	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Electrocution()
{

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "ragdoll@human";
	char *animID = "electrocute";


	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Suicide()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mp_suicide";
	char *animID = "pistol";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Showering()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mp_safehouseshower@male@";
	char *animID = "male_shower_idle_b";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
}
void Dog()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "missfra0_chop_find";
	char *animID = "hump_loop_chop";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}

void meditation()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmcollect_paperleadinout@";
	char *animID = "meditiate_idle";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}

void SexTake()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmpaparazzo_2";
	char *animID = "shag_loop_poppy";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}
void onfire()
{
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "random@arrests";
	char *animID = "kneeling_arrest_idle";

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(playerPed, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}

//////////////////////////////////animais



void Boar()
{

	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_BOAR_GRAZING", 0, true);

}
void Cat()
{

	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_CAT_SLEEPING_GROUND", 0, true);

}
void Cow()
{

	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_COW_GRAZING", 0, true);

}
void Coyote()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_COYOTE_HOWL", 0, true);


}
void Coyote2()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_COYOTE_WANDER", 0, true);

}


void Chicken()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_CHICKENHAWK_FEEDING", 0, true);


}
void Deer()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_DEER_GRAZING", 0, true);



}
void Rottweiler()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_DOG_BARKING_ROTTWEILER", 0, true);


}
void Retreiver()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_DOG_BARKING_RETRIEVER", 0, true);

}
void Shepard()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_DOG_BARKING_SHEPHERD", 0, true);

}
void Lion1(){
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_MOUNTAIN_LION_REST", 0, true);
}


void Lion2()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_MOUNTAIN_LION_WANDER", 0, true);

}

void Pig()
{
	AI::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), "WORLD_PIG_GRAZING", 0, true);

}
















//////////////////////////////////////////////
/*///////////////////////////////////////////////*/// senarios



void Musician()
{
	char *anim = "WORLD_HUMAN_MUSICIAN";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Welding()
{
	char *anim = "WORLD_HUMAN_WELDING";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Lifting()
{
	char *anim = "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}

void Gardener()
{
	char *anim = "WORLD_HUMAN_GARDENER_LEAF_BLOWER";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Binoculars()
{
	char *anim = "WORLD_HUMAN_BINOCULARS";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Bum()
{
	char *anim = "WORLD_HUMAN_BUM_FREEWAY";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void High()
{
	char *anim = "WORLD_HUMAN_PROSTITUTE_HIGH_CLASS";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Smoke()
{
	char *anim = "WORLD_HUMAN_AA_SMOKE";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Hammering()
{

	char *anim = "WORLD_HUMAN_HAMMERING";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);

}
void Maid()
{
	char *anim = "WORLD_HUMAN_MAID_CLEAN";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Paparazzi()
{
	char *anim = "WORLD_HUMAN_PAPARAZZI";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}

void Strip()
{
	char *anim = "PROP_HUMAN_SEAT_STRIP_WATCH";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Partying()
{
	char *anim = "WORLD_HUMAN_PARTYING";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Smoking()
{
	char *anim = "WORLD_HUMAN_SMOKING_POT";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Stupor()
{
	char *anim = "WORLD_HUMAN_STUPOR";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Sumbathe()
{
	char *anim = "WORLD_HUMAN_SUNBATHE";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}

void Tourist()
{
	char *anim = "WORLD_HUMAN_TOURIST_MOBILE";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
void Statue()
{
	char *anim = "WORLD_HUMAN_HUMAN_STATUE";
	AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
	AI::TASK_START_SCENARIO_IN_PLACE(playerPed, anim, 0, true);


}
////////////////////////////////
///powers

bool forcewave = false;
void ForceWave() {

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_reconstructionaccident");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_reconstructionaccident");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_reconstruct_pipe_impact", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
	Vehicle veh[arrSize];
	veh[0] = numElements;
	int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), veh);
	if (veh != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(veh[offsettedID], 1, 9, 9, 0, 0, 0, 0, 1, true, true, true, true, true);

				char *dict = "rcmcollect_paperleadinout@";
				char *anim = "meditiate_idle";
				STREAMING::REQUEST_ANIM_DICT(dict);
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
			}
		}
	}
}

void ForceExplodeWave()
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Vehicle veh[arrSize];
	veh[0] = numElements;
	int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), veh);
	if (veh != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
			{
				Vector3 Pos = ENTITY::GET_ENTITY_COORDS(veh[offsettedID], true);
				FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z, 29, 0.5f, true, true, 1.0f);

				char *dict = "rcmcollect_paperleadinout@";
				char *anim = "meditiate_idle";
				STREAMING::REQUEST_ANIM_DICT(dict);
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
			}
		}
	}
}


void MCDance()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);
				char *dict = "mini@strip_club@private_dance@part1";
				char *anim = "priv_dance_p1";
				STREAMING::REQUEST_ANIM_DICT(dict);
				AI::TASK_PLAY_ANIM(ped[offsettedID], dict, anim, 1, 1, -1, 16, 0, false, 0, false);
			}
		}
	}
}

void GiveWeaponPed(int Ped, int WeaponHash)
{
	WEAPON::GIVE_WEAPON_TO_PED(Ped, WeaponHash, 9999, 1, 1);

}

void DeletePed(int pedID)
{
	if (!NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(pedID))
		while (!NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(pedID));

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pedID, 1, 1);
	PED::DELETE_PED(&pedID);
}

void makePedHostile(uint Ped, int Target, bool choporswat = false, bool isClone = false)
{
	if (choporswat)
	{
		AI::TASK_COMBAT_PED(Ped, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0, 16);
		PED::SET_PED_CAN_SWITCH_WEAPON(Ped, true);
	}
	else
	{
		if (isClone)
		{
			int Hash = ENTITY::GET_ENTITY_MODEL(Ped);
			DeletePed(Ped);

			STREAMING::REQUEST_MODEL(Hash);
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Target), 1);
			if (STREAMING::HAS_MODEL_LOADED(Hash))
			{
				int newPed = PED::CREATE_PED(21, Hash, coords.x, coords.y, coords.z, 0, 1, 0);
				if (ENTITY::GET_ENTITY_HEALTH(newPed) == ENTITY::GET_ENTITY_MAX_HEALTH(newPed))
				{
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hash);
					RequestNetworkControl(newPed);
					AI::TASK_COMBAT_PED(newPed, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 0, 16);
					PED::SET_PED_CAN_SWITCH_WEAPON(Ped, true);
				}
			}
		}
		else
		{
			int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Target);
			AI::TASK_COMBAT_PED(Ped, TargetHandle, 0, 16);
		}
	}
}

bool mcriot = false;
void MCRiot()
{
	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);
				GiveWeaponPed(ped[offsettedID], 0x47757124);
				makePedHostile(ped[offsettedID], TargetHandle, true);
			}
		}
	}
}

void MCFollow()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);

				int my_group = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_ID());
				PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), my_group);
				PED::SET_PED_AS_GROUP_MEMBER(ped[offsettedID], my_group);
			}
		}
	}
}

void MCPushups()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);
				char *dict = "amb@world_human_push_ups@male@base";
				char *anim = "base";
				STREAMING::REQUEST_ANIM_DICT(dict);
				AI::TASK_PLAY_ANIM(ped[offsettedID], dict, anim, 1, 1, -1, 16, 0, false, 0, false);
			}
		}
	}
}

void MCBurn()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);

				Vector3 Pos = ENTITY::GET_ENTITY_COORDS(ped[offsettedID], true);
				FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), Pos.x, Pos.y, Pos.z, 12, 5, true, true, 0);

			}
		}
	}
}

void MCWater()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);

				Vector3 Pos = ENTITY::GET_ENTITY_COORDS(ped[offsettedID], true);
				FIRE::ADD_OWNED_EXPLOSION(PLAYER::PLAYER_PED_ID(), Pos.x, Pos.y, Pos.z, 13, 5, false, true, 0);

			}
		}
	}
}
void MCPush()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Ped ped[arrSize];
	ped[0] = numElements;
	int count = PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), ped, 1);
	if (ped != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (ped[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(ped[offsettedID]))
			{
				int TargetHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
				RequestNetworkControl(ped[offsettedID]);

				ENTITY::APPLY_FORCE_TO_ENTITY(ped[offsettedID], 1, 9, 9, 0, 0, 0, 0, 1, true, true, true, true, true);

				PED::SET_PED_CAN_RAGDOLL(ped[offsettedID], true);
				PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped[offsettedID], true);
				PLAYER::GIVE_PLAYER_RAGDOLL_CONTROL(ped[offsettedID], true);
				PED::SET_PED_RAGDOLL_ON_COLLISION(ped[offsettedID], true);

			}
		}
	}
}
void ForceBoost()
{

	const int numElements = 10;
	const int arrSize = numElements * 2 + 2;
	Vehicle veh[arrSize];
	veh[0] = numElements;
	int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), veh);
	if (veh != NULL)
	{
		for (int i = 1; i <= count; i++)
		{
			int offsettedID = i;
			if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
			{
				RequestNetworkControl(veh[offsettedID]);
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh[offsettedID], 100.0f);
			}
		}
	}
}


char* SoundToPlay;
char* SoundSetToPlay;
bool PlaySounds;



void Force1()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_disintegrate", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	ForceExplodeWave();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}


void Force2()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_disintegrate", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	ForceWave();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}

void Force3()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
	MCDance();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}

void Force4()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
	MCRiot();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}

void Force5()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
	MCFollow();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}

void Force6()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
	MCPushups();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}

void Force7()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_disintegrate", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	MCBurn();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;


}

void Force8()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_disintegrate", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	MCWater();
	char *dict = "rcmcollect_paperleadinout@";//////////////////////////////////////////////////////	 trailer splash
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}

void Force9()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_reconstructionaccident");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_reconstructionaccident");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_reconstruct_pipe_impact", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	MCPush();
	char *dict = "switch@trevor@pushes_bodybuilder";
	char *anim = "001426_03_trvs_5_pushes_bodybuilder_exit_trv";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;

}
void Force10()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_reconstructionaccident");
	GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_reconstructionaccident");
	GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_reconstruct_pipe_impact", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	ForceBoost();
	char *dict = "rcmcollect_paperleadinout@";
	char *anim = "meditiate_idle";
	STREAMING::REQUEST_ANIM_DICT(dict);
	AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), dict, anim, 1, 1, -1, 16, 0, false, 0, false);
	SoundToPlay = "FocusIn";
	SoundSetToPlay = "HintCamSounds";
	PlaySounds = true;


}




//////////////////////////////////////////////////walk types


char* WalkType;
bool changingWalkType;


void   Male()
{

		WalkType = "move_m@generic";
		changingWalkType = true;

}

void   Female()
{
	WalkType = "move_f@generic";
	changingWalkType = true;
}
void   Drunk()
{

	WalkType = "move_m@drunk@verydrunk";
	changingWalkType = true;
}
void   Fatass()
{
	WalkType = "move_m@fat@a";
	changingWalkType = true;
}
void   Business()
{
	WalkType = "move_m@business@a";
	changingWalkType = true;

}
void   Hiker()
{
	WalkType = "move_m@hiking";
	changingWalkType = true;
}
void   Running()
{
	WalkType = "move_f@film_reel";
	changingWalkType = true;

}
void   Limp()
{
	WalkType = "move_m@injured";
	changingWalkType = true;

}


/////////clone



void   Pole2()
{
	Player player = PLAYER::PLAYER_ID();
	
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mini@strip_club@pole_dance@pole_dance2";
	char *animID = "pd_dance_02";

	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}
void   Meditation()
{
	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "amb@world_human_push_ups@male@base";
	char *animID = "base";

	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}

void   Dance2()
{
	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Hash fireworkl = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK");
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmcollect_paperleadinout@";
	char *animID = "meditiate_idle";

	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);
	PED::SET_PED_COMBAT_ABILITY(clone, 100);
	WEAPON::GIVE_WEAPON_TO_PED(clone, fireworkl, fireworkl, 9999, 9999);
	PED::SET_PED_CAN_SWITCH_WEAPON(clone, true);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	
}
void   Teste2()
{
	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mini@prostitutes@sexnorm_veh";
	char *animID = "bj_loop_male";

	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);

}
void   Teste3()
{

	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmfanatic1celebrate";
	char *animID = "celebrate";

	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	
}
void t4() {

	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mp_suicide";
	char *animID = "pistol";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);


}
void t5() {
	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "mp_safehouseshower@male@";
	char *animID = "male_shower_idle_b";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);


}
void t6() {
	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmcollect_paperleadinout@";
	char *animID = "meditiate_idle";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}
void t7() {

	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	char *anim = "rcmpaparazzo_2";
	char *animID = "shag_loop_poppy";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}
void t8() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "random@arrests";
	char *animID = "kneeling_arrest_idle";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}
//aki  
void t9() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "random@arrests";
	char *animID = "rcm_fanatic2";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}

void t10() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "random@arrests";
	char *animID = "thanks_male_05";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}

void t11() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "random@crash_rescue@dead_ped";
	char *animID = "dead_ped";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}

void t12() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "random@drunk_driver_2";
	char *animID = "cardrunksex_loop_f";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}

void t13() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "explosions_run_a";
	char *animID = "run";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}

void t14() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "missarmenian2lamar_in_cover";
	char *animID = "lamar_explosion_react";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}

void t15() {


	Player player = PLAYER::PLAYER_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;
	Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

	char *anim = "missarmenian3";
	char *animID = "tennis_coach_loop_wife";
	int clone = PED::CLONE_PED(playerPed, pos.x, pos.y, pos.z);
	ENTITY::SET_ENTITY_INVINCIBLE(clone, false);

	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);

	AI::TASK_PLAY_ANIM(clone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);



}


















//////////////////////////////////////////////////



//Time Options
void HourForw()
{
	int sample_hour = TIME::GET_CLOCK_HOURS();
	if (sample_hour + 1 == 24) NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(0, 0, 0);
	else NETWORK::NETWORK_OVERRIDE_CLOCK_TIME((sample_hour + 1), 0, 0);
}
void HourBack()
{
	int sample_hour = TIME::GET_CLOCK_HOURS();
	if ((sample_hour - 1) == -1) NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(23, 0, 0);
	else NETWORK::NETWORK_OVERRIDE_CLOCK_TIME((sample_hour - 1), 0, 0);
	return;
}

//weather


void ExtraSunny()
{
	GAMEPLAY::SET_OVERRIDE_WEATHER("EXTRASUNNY");
}
void Sunny() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("CLEAR");
}
void Cloudy() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("CLOUDS");
}

void Smoggy() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("SMOG");
}

void Foggy() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("FOGGY");
}
void  overcast() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("OVERCAST");
}
void Stormy() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("THUNDER");
}
void snow() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("SNOW");
}

void snowlight() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("SNOWLIGHT");
}
void  blizzard() {
	GAMEPLAY::SET_OVERRIDE_WEATHER("BLIZZARD");
}



//Vehicle Options
/*void maxvehicle
{
	Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	for (int i = 0; i < 50; i++)
	{
		VEHICLE::SET_VEHICLE_MOD(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1, false);
	}
	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, "ev0lution");
}*/

void FiXVehicle(Vehicle veh)
{
	if (ENTITY::DOES_ENTITY_EXIST(veh)) {

		VEHICLE::SET_VEHICLE_FIXED(veh);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, TRUE, TRUE, TRUE);
	}
	else
		notifyMap("~r~Error: Player Not In Vehicle");
}
void PaintRandom(Vehicle veh)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
		if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, rand() % 255, rand() % 255, rand() % 255);
	}
}
void FlipCar(Vehicle veh)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
	}
}

void SpawnVehicle(Hash hash)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(hash))
	{
		STREAMING::REQUEST_MODEL(hash);
		while (!STREAMING::HAS_MODEL_LOADED(hash)) WAIT(0);
		Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 6.0, 0.5);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
		PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		notifyMap("Spawned vehicle");
	}
	else
		notifyMap("Vehicle doesn't exist");
}





void applyForceToEntity(Entity entity, float x, float y, float z)
{
	ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, x, y, z, 0.0f, 0.0f, 0.0f, 0, TRUE, TRUE, TRUE, FALSE, TRUE);
}

static LPCSTR weaponNames[] = {
	"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
	"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
	"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
	"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
	"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
	"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
	"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN",
	"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
	"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
	"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN",
	"WEAPON_COMBATPDW", "WEAPON_KNUCKLE", "WEAPON_MARKSMANPISTOL"
};

void  giveweap()
{
	for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY((char *)weaponNames[i]), 9999, 0);



};
void giveweapmk2()
{


		const static Hash weaponList[] = { 3219281620 ,2024373456 ,961495388 ,4208062921 ,3686625920 ,177293209 ,};

	for each (Hash weapon in weaponList)
	{
		WEAPON::GIVE_WEAPON_TO_PED(playerPed, weapon, 9999, FALSE, TRUE);
	}




}
void shootAtPed(Player selectedPlayer)
{
	Vector3 Mouth = PED::GET_PED_BONE_COORDS(selectedPlayer, SKEL_Head, 0.1f, 0.0f, 0.0f);
	PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), Mouth.x, Mouth.y, Mouth.z, true);
}
//on players 


void SpectatePlayer(Ped playerped)
{
	Any SpectateCam;

	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerped, FALSE);
	SpectateCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, 0.0, 0.0, 0.0, 50.0, 0, 2);
	CAM::ATTACH_CAM_TO_ENTITY(SpectateCam, playerped, 0, -5, 1, 1);
	//CAM::ATTACH_CAM_TO_PED_BONE(hi, selectedPed, 0, 0, -5, 1, 1);
	//CAM::POINT_CAM_AT_PED_BONE(hi, selectedPed, 0, 0, 0.7, 0, 1);
	//CAM::SET_CAM_FOV(hi, 50.0);
	CAM::SET_CAM_ACTIVE(playerped, 1);
}

void SpectateMode(bool Active, Ped selectedPed)
{
	NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(Active, selectedPed);
}

void AIWanderCar(Ped = playerPed)
{
	Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	AI::TASK_VEHICLE_DRIVE_WANDER(playerPed, playerVeh, 100.0f, 0);
}

int SelectedClient;
void TeleportToClient(int Client)
{
	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(Client, 1);
	if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID()))
		ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), Coords.x, Coords.y, Coords.z, 1, 0, 0, 1);
	else
		ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z, 1, 0, 0, 1);


}

void Tptoplayer()
{
	TeleportToClient(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));

}
bool hijack = false;

void TptoplayerCar()

{
	Player player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(selectedPlayer);

	Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(selectedPlayer, 0);
	TeleportToClient(PLAYER::PLAYER_PED_ID());
	ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer)));
	if (hijack) {
		PED::SET_PED_INTO_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), veh, 0);
	}
	else {
		PED::SET_PED_INTO_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), veh, -1);
	}




}

void GiveAllWeapons(Ped ped)
{
	const static Hash weaponList[] = { 0x99B507EA, 0x678B81B1, 0x4E875F73,0x958A4A8F, 0x440E4788, 0x84BD7BFD,0x1B06D571, 0x5EF9FEC4,0x22D8FE39, 0x99AEEB3B, 0x13532244,0x2BE6766B,0xEFE7E2DF,0x0A3D4D34,0xBFEFFF6D, 0x83BF0278,0xAF113F99,0x9D07F764,0x7FD62962,0x1D073A89,0x7846A318,0xE284C527,0x9D61E50F,0x3656C8C1,0x5FC3C11,0xC472FE2,0xA284510B,0x4DD2DC56,0xB1CA77B1, 0x42BF8A85,0x93E220BD,0x2C3731D9,0xFDBC8A50,0xA0973D5E,0x24B17070,0x60EC506,0x34A67B97,0xBFD21232,0xC0A3098D, 0xC734385A,0x3AABBBAA,0x61012683,0xF9DCBF2D, 0x6D544C99,0xA2719263 };

	for each (Hash weapon in weaponList)
	{
		WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, 9999, FALSE, TRUE);
	}
}



void GiveAllWeapons()
{

	GiveAllWeapons(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));


}

void TakeAllWeapons()
{
	WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true);


}

void KickP()
{


	NETWORK::NETWORK_SESSION_KICK_PLAYER(selectedPlayer);


}
void Blame()
{
	
	for (int i = 0; i <= 32; i++)
	{
		
		if (i == PLAYER::PLAYER_ID())continue;
		int Handle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(Handle, 1);
		if (!ENTITY::DOES_ENTITY_EXIST(Handle)) continue;
		

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(pos.x, pos.y, pos.z + 35, pos.x, pos.y, pos.z, 250, 1, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1, 1, 500);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(pos.x, pos.y, pos.z + 55, pos.x, pos.y, pos.z, 100, false, 37, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true, true, 600);
	
	


		{
			if (i == 32)

			{
				break;
			}
		

		}
	}

	




}

void frame()
{


	
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
	{
		for (int i = 0; i < 32; i++)
		{
			if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == PLAYER::PLAYER_PED_ID()) continue;
			{
				Player playerHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0.0, 0.0, 0.0);
				PLAYER::RESERVE_ENTITY_EXPLODES_ON_HIGH_EXPLOSION_COMBO(selectedPlayer, 1);

			}
		}
	}




	
}




void RequestingControl(Entity e)
{
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(e);
	if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(e))
		WAIT(0);
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(e);
}





void pclone()
{


	PED::CLONE_PED(selectedPlayer, 1, 1, 1);


}



//////////////////////////////////tROLLL 
void Killlayer()
{
	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 29, (Damage_), true, false, 5.0f);

}
void SeilentKilllayer()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 29, (Damage_), false, true, 0.0f);




}

void AirstrikeKilllayer()
{
	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	Coords.z += 15;
	GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 35, Coords.x, Coords.y, Coords.z, 250, 1, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1, 1, 500);
	GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 30, Coords.x, Coords.y, Coords.z, 250, 0, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
	GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 55, Coords.x, Coords.y, Coords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true, true, 600);




}


void PedsRiot()
{
	char *anim = "mini@prostitutes@sexnorm_veh";
	char *animID = "bj_loop_male";
	int playerClone = PED::CLONE_PED(playerPed, 1, 1, 1);
	ENTITY::SET_ENTITY_INVINCIBLE(playerClone, true);
	STREAMING::REQUEST_ANIM_DICT(anim);
	while (!STREAMING::HAS_ANIM_DICT_LOADED(anim))
		WAIT(0);
	AI::TASK_PLAY_ANIM(playerClone, anim, animID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	ENTITY::ATTACH_ENTITY_TO_ENTITY(playerClone, playerPed, 0.059999998658895f, 0.0f, -0.25f, 0.0f, 0.0f, 0.0f, 1, 1, 0, 0, 2, 1, 1);
}


//Tipos  de explosao

void kill0()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 0, (Damage_), true, false, (Shake_));




}
void kill1()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 1, (Damage_), true, false, (Shake_));




}

void kill2()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 2, (Damage_), true, false, (Shake_));




}
void kill3()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 3, (Damage_), true, false, (Shake_));




}
void kill4()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 4, (Damage_), true, false, (Shake_));




}
void kill5()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 5, (Damage_), true, false, (Shake_));




}
void kill6()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 6, (Damage_), true, false, (Shake_));




}
void kill7()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 7, (Damage_), true, false, (Shake_));




}
void kill8()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 8, (Damage_), true, false, (Shake_));




}
void kill9()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 9, (Damage_), true, false, (Shake_));


}
void kill10()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 10, (Damage_), true, false, (Shake_));




}

void kill11()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 11, (Damage_), true, false, (Shake_));




}
void kill12()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 12, 0.5f, true, false, (Shake_));




}
void kill13()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 13, (Damage_), true, false, (Shake_));




}
void kill14()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 14, (Damage_), true, false, (Shake_));




}
void kill15()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 15, (Damage_), true, false, (Shake_));




}
void kill16()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 16, (Damage_), true, false, (Shake_));




}
void kill17()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 17, (Damage_), true, false, (Shake_));




}
void kill18()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 18, (Damage_), true, false, (Shake_));




}
void kill19()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 19, (Damage_), true, false, (Shake_));




}
void kill20()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 20, (Damage_), true, false, (Shake_));




}
void kill21()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 21, (Damage_), true, false, (Shake_));




}
void kill22()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 22, (Damage_), true, false, (Shake_));




}
void kill23()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 23, (Damage_), true, false, (Shake_));




}
void kill24()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 24, (Damage_), true, false, (Shake_));




}
void kill25()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 25, (Damage_), true, false, (Shake_));




}
void kill26()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 26, (Damage_), true, false, (Shake_));




}
void kill27()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 27, (Damage_), true, false, (Shake_));




}
void kill28()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 28, (Damage_), true, false, (Shake_));




}
void kill29()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 29, (Damage_), true, false, (Shake_));




}
void kill30()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 30, (Damage_), true, false, (Shake_));




}
void kill31()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 31, (Damage_), true, false, (Shake_));




}
void kill32()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 32, (Damage_), true, false, (Shake_));




}
void kill33()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 33, (Damage_), true, false, (Shake_));




}
void kill34()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 34, (Damage_), true, false, (Shake_));




}
void kill35()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 35, (Damage_), true, false, (Shake_));




}
void kill36()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 36, (Damage_), true, false, (Shake_));




}
void kill37()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 37, (Damage_), true, false, (Shake_));




}
void kill38()
{


	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
	FIRE::ADD_EXPLOSION(Coords.x, Coords.y, Coords.z, 38, (Damage_), true, false, (Shake_));




}

void  featureloop()
{


	Player playerPed2 = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
	Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed2, 0);
	FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, (Explosion_), (Damage_), true, false, (Shake_));


}





void aaaa()

{


	MOBILE::CREATE_MOBILE_PHONE(1);





}
void destruir()

{

	MOBILE::DESTROY_MOBILE_PHONE();

}


void Ccarea1()
{



	Player player = PLAYER::PLAYER_ID();
	Ped playerPed3 = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed3, 1);
	GAMEPLAY::CLEAR_AREA(position.x, position.y, position.z, 999999999.f, 0, 0, 0, 0);


}

void Ccarea2()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed3 = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed3, 1);
	GAMEPLAY::CLEAR_AREA_OF_VEHICLES(position.x, position.y, position.z, 999999999.f, 0, 0, 0, 0, 0);




}
void Ccarea3()
{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed3 = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed3, 1);
	GAMEPLAY::CLEAR_AREA_OF_PROJECTILES(0x0A1CB9094635D1A6, position.x, position.y, position.z, 999999999.f);


}


void Ccarea4()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed3 = PLAYER::PLAYER_PED_ID();
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed3, 1);
	GAMEPLAY::CLEAR_AREA_OF_VEHICLES(position.x, position.y, position.z, 999999999.f, 0, 0, 0, 0, 0);


}
void Ccarea5()

{

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed3 = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed3);
	Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed3, 1);
	GAMEPLAY::CLEAR_AREA_OF_COPS(position.x, position.y, position.z, 999999999.f, -1);


}
void reset()
{


	WATER::_RESET_WAVES_INTENSITY();

}







//self

bool featurePlayerInvincible = false;//1
bool featureNoRagDoll = false;//2
bool featureNoRagDollUpdated = false;
bool featurePlayerInvincibleUpdated = false;//3
bool featurePlayerNeverWanted = false;//4
bool featurePlayerFastRun = false;//5
bool featurePlayerFastRunUpdated = false;//5
bool featurePlayerUltraRun = false;//6
bool featurePlayerRunScale = false;
bool featurePlayerUltraUpdated = true;//6
bool featurePlayerSuperJump = false;//7
bool featurePlayerUltraJump = false;//8
bool featureMiscFlyMode = false;//9
bool featurePlayerInvisible = false;//10
bool featurePlayerInvisibleUpdated = false;//10
bool featurePlayeroffradar = false;//11
bool featurePlayerslowmotion = false;//12
bool featurePlayerslowmotionUpdated = false; //13
bool featureForcefiel = false; //14
bool featurefastswim = false; //15
bool featurefastswimUpdated = false;
bool featurePlayerScale = false;
bool featurePlayerScaleUpdated = false;
bool featureOpaci = false;
bool featureWalk = false;
bool featureRPLoop = false;
bool featureclip = false;
bool featureclip2 = false;
bool feature99m = false;
bool feature15m = false;
bool featureAnticrash = false;
bool feature10m = false;
bool feature5m = false;
bool featurePlayerIgnored = false;
bool featurePlayerIgnoredUpdated = false;
bool featurePlayerUnlimitedAbility = false;
bool featureMassSuicideLoop = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastSwimUpdated = false;
bool featurePlayerolaghimout = false;
bool featureHealPlayer = false;
bool featureallweapon = false;
bool featurekill = false;
bool featureExplodeSelectedPlayer = false;
bool featureWeaponNoReload = false;
bool featureWeaponFireAmmo = false;
bool featureWeaponExplosiveAmmo = false;
bool featureWepModMolotov = false;
bool featurePlayermoneyDrop = false;
bool featuremobileopmult = false;
bool featurelaghimout = false;
bool featurenoclipSafety = false;
bool featureWeaponExplosiveMelee = false;
bool featureWeaponOneHit = false;
bool featureWeaponRainbowTint = false;
bool featureVehScrollingText = false;
bool featureVehSlide = false;
bool DeleteVehicle = false;
bool maxvehicle = false;
bool featureVehSuperRPM = false;
bool featureTeleportGun = false;
bool featureonehit = false;
bool featurerapidfire = false;
bool featurerapidMultiplier = false;
bool featureRecoil = false;
bool featureMolotov = false;
bool featureFlare = false;
bool featureWeaponFirework = false;
bool featureWeaponnuke = false;
bool featureWepModVehGun = false;
bool featureSuperPunch = false;
bool featuretrigger = false;
bool featureaim = false;
bool featureWeaponRPG = false;
bool featureVehInvincible = false;
bool featureVehInvincibleUpdated = false;
bool featureVehSeatbelt = false;
bool featureVehSeatbeltUpdated = false;
bool featureVehSpeedBoost = false;
bool featureVehRainbow = false;
bool featureWorldMoonGravity = false;
bool featureBlackout = false;
bool featurehidhub = false;
bool featurewave = false;
bool featureLaunch = false;
bool featureMeteor = false;
bool featureMeteorUpdated = false;
//onplayers
bool featureSpectate = false;
bool featureSpectateUpdate = false;
bool featurefreezeplayer = false;
bool featureShakecam = false;
bool featureShakecamUpdated = false;
bool featureCash25 = false;
bool featureCash = false;
bool featureCashfake = false;
bool featureCash2 = false;
bool featureCash3 = false;
bool featureCash4 = false;
bool featureCash5 = false;
bool featureforce = false;
bool featureBurn = false;
bool featureAirstrike = false;
bool featureCrashClone = false;
bool featureCrashClone2 = false;
bool featureFire = false;
bool featureFire2 = false;
bool featureESP = false;
bool featureTimePaused = false;
bool featureTimePausedUpdated = false;
bool featureTimeSynced = false;
bool featureSnow = false;
bool featureMiscLockRadio = false;
bool featurDisable = false;
bool featureMiscHideHud = false;
bool featureMiscMobile = false;
bool stealth15MLoopStarted = false;
bool featureVehWrapInSpawned = true;
bool MoneyDrop = false;
bool featureFakeBags = false;
bool featureRhinoGun = false;
bool featurePantoGun = false;
bool featureExplosionl = false;




void update_features()
{
	//Common Variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();

	//Self Options

	//=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=////=//

	//=//1
	if (featurePlayerInvincible == false)

	{
		if (bPlayerExists && !featurePlayerInvincible)
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);

		featurePlayerInvincibleUpdated = false;
	}

	if (featurePlayerInvincible)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);

	}
	//=//2
	if (featureNoRagDoll)
	{
		Ped playerPedID = PLAYER::PLAYER_PED_ID();

		PLAYER::GIVE_PLAYER_RAGDOLL_CONTROL(playerPedID, 1);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPedID, false);
		PED::SET_PED_CAN_RAGDOLL(playerPedID, false);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPedID, 0);
		PED::SET_PED_RAGDOLL_ON_COLLISION(playerPedID, 0);


	}
	if (featureNoRagDollUpdated)

	{
		Ped playerPedID = PLAYER::PLAYER_PED_ID();

		PLAYER::GIVE_PLAYER_RAGDOLL_CONTROL(playerPedID, 1);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPedID, true);
		PED::SET_PED_CAN_RAGDOLL(playerPedID, true);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPedID, 0);
		PED::SET_PED_RAGDOLL_ON_COLLISION(playerPedID, 0);

	}

	//=//3

	if (featurePlayerNeverWanted)
	{
		if (bPlayerExists)
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
	}

	if (featurePlayerIgnored)
	{
		if (bPlayerExists)
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, featurePlayerIgnored);
		featurePlayerIgnoredUpdated = false;
	}


	//=//4


	if (featurePlayerFastRun)
	{
		if (!featurePlayerFastRun)
		{

			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.00);
		}
		else
		{

			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.39);
		}



	}
	//=//5

	if (featurePlayerUltraRun)
	{

		if (IsKeyPressed('W')) {
			Ped ped = PLAYER::PLAYER_PED_ID();
			Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0, 0.6, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.0f, 1.3, 0, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			PLAYER::SET_PLAYER_SPRINT(PLAYER::PLAYER_ID(), 1);
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.59);

		}

	}

	if (featurePlayerRunScale)
	{

		if (IsKeyPressed('W')) {
			Ped ped = PLAYER::PLAYER_PED_ID();
			Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0, 0.6, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0.0f, 1.3, 0, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			PLAYER::SET_PLAYER_SPRINT(PLAYER::PLAYER_ID(), (Scale));
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), (Scale));

		}

	}
	//=//6

	if (featurePlayerSuperJump)
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_PED_ID());
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());

	}

	if (featurePlayerUltraJump)
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
		WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE"));
		if (PED::IS_PED_JUMPING(playerPed))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 0, (Multi), 0, 0, 0, true, true, true, true, false, true);
		}

	}


	//=//7

	if (featureMiscFlyMode)
	{
		Player player = PLAYER::PLAYER_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool fUp = IsKeyPressed(VK_OEM_4);
		bool fFowrd = IsKeyPressed(VK_OEM_6);
		//	Ped playerPed = PLAYER_PED_ID();
		//	PrintStringBottomCentre("~g~FLY UP~s~(-) | ~g~FLY FORWARD~s~(7)");

		if (fUp || fFowrd)
		{
			if (fUp) {
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
				PED::SET_PED_TO_RAGDOLL_WITH_FALL(playerPed, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
				ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 0, 9999999, 0, 0, 0, true, true, true, true, false, true);

			}
			else
				if (ENTITY::IS_ENTITY_IN_AIR(playerPed))
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, true, 0, 9999999, 0, 0, 0, 0, true, true, true, true, false, true);
		}

	}



	//=//8

	if (featurePlayerInvisible == true)
	{
		featurePlayerInvisible = true;

		ENTITY::SET_ENTITY_VISIBLE(playerPed, false);



	}

	if (featurePlayerInvisible == false)
	{
		featurePlayerInvisible = false;

		ENTITY::SET_ENTITY_VISIBLE(playerPed, true);
	}

	//=//9





	if (featurePlayeroffradar)
	{



	}

	//=//10

	if (featurePlayerslowmotion == true)
	{

		if (bPlayerExists)
		{
			featurePlayerslowmotion = true;
			GAMEPLAY::SET_TIME_SCALE(0.2);

		}
	}

	if (featurePlayerslowmotionUpdated == false)

	{
		if (bPlayerExists)
		{
			featurePlayerslowmotionUpdated = false;

			GAMEPLAY::SET_TIME_SCALE(1);
		}

	}
	//14

	if (featureForcefiel) {


		Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 7, 9.0f, false, true, 0.0f);




	}

	if (featurefastswimUpdated)
	{
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.00);
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_PED_ID(), 1.00);


	}


	if (featurefastswim)
	{

		featurefastswim = true;
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), (Swim));
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_PED_ID(), (Swim));


	}
	if (featurePlayerScale)
	{




		PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 223, 20);




		//(PLAYER::PLAYER_PED_ID(), (Scale), 1);


	}

	if (featurePlayerScale == false)
	{


		PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), 0, 0);



	}

	if (featureOpaci)
	{

		ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), (Opacy), false);



	}



	if (featureWalk)
	{
		char* prop = "p_oil_slick_01";
		Hash ObjHash = GAMEPLAY::GET_HASH_KEY(prop);
		Vector3 PlayerCoord = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		Object WalkOnAir = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(PlayerCoord.x, PlayerCoord.y, PlayerCoord.z, 100, ObjHash, 1, 0, 1);
		float zPlane;

		if ((!ENTITY::DOES_ENTITY_EXIST(WalkOnAir)) && (featureWalk))
		{
			Object WalkOnAir = Create_Obj(prop, PlayerCoord);
			ENTITY::FREEZE_ENTITY_POSITION(WalkOnAir, 1);
			ENTITY::SET_ENTITY_VISIBLE(WalkOnAir, 0);
		}
		else if ((ENTITY::DOES_ENTITY_EXIST(WalkOnAir)) && (featureWalk))
		{
			float Normal = 1.0225;
			float Down = 1.60;
			float Up = 0.40;
			float Vehicular = 50.0;

			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))       //Gone when in a Car
			{
				zPlane = PlayerCoord.z - Vehicular;
			}
			else if (GetAsyncKeyState(0x5A) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_SCRIPT_LS)))      //Down Z and L3          
			{
				zPlane = PlayerCoord.z - Down;
			}
			else if (GetAsyncKeyState(0x51) || (CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_SCRIPT_RS)))      //Up Q and R3  
			{
				zPlane = PlayerCoord.z - Up;
			}
			else                                              //Normal
			{
				zPlane = PlayerCoord.z - Normal;
			}

			Vector3 Coord = CombineVector(PlayerCoord.x, PlayerCoord.y, zPlane);
			ENTITY::SET_ENTITY_COORDS(WalkOnAir, Coord.x, Coord.y, Coord.z, 1, 0, 0, 1);
		}








	}
	if (featureRPLoop)
	{

		static int i = 0;

		if (i % 2 == 0)
		{
			if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) == 0)
			{
				PLAYER::REPORT_CRIME(player, 36, PLAYER::GET_WANTED_LEVEL_THRESHOLD(5));
				PLAYER::SET_WANTED_LEVEL_DIFFICULTY(player, 0.0f);
				PLAYER::SET_PLAYER_WANTED_CENTRE_POSITION(player, -99999, -99999, 99999);
				PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(player, FALSE);
			}
			else
			{
				PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
			}
		}
		i++;


	}
	if (featureclip)
	{
		ENTITY::SET_ENTITY_VISIBLE(playerPed, false);
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		if (GetAsyncKeyState(VK_KEY_S) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 268)) {
			float fivef = .5f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			float xVec = fivef * sin(degToRad(heading)) * -1.0f;
			float yVec = fivef * cos(degToRad(heading));
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.x -= xVec, pos.y -= yVec;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}
		if (GetAsyncKeyState(VK_KEY_W) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 269)) {
			float fivef = 8.10f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			float xVec = fivef * sin(degToRad(heading)) * -5.0f;
			float yVec = fivef * cos(degToRad(heading));
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.x += xVec, pos.y += yVec;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}
		if (GetAsyncKeyState(VK_KEY_A) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 266)) {
			float fivef = 5.50f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading + 5.50f);
		}
		if (GetAsyncKeyState(VK_KEY_D) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 271)) {
			float fivef = 5.50f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading - 5.50f);
		}
		if (GetAsyncKeyState(VK_SHIFT) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, ControlFrontendRb)) {
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.z -= 5.5;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}
		if (GetAsyncKeyState(VK_SPACE) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, ControlFrontendLb)) {
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.z += 5.5;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}



	}
	if (featureclip == FALSE)
	{

		ENTITY::SET_ENTITY_VISIBLE(playerPed, true);




	}
	if (featureclip2)
	{

		Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		if (GetAsyncKeyState(VK_KEY_S) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 268)) {
			float fivef = .5f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			float xVec = fivef * sin(degToRad(heading)) * -1.0f;
			float yVec = fivef * cos(degToRad(heading));
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.x -= xVec, pos.y -= yVec;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}
		if (GetAsyncKeyState(VK_KEY_W) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 269)) {
			float fivef = .5f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			float xVec = fivef * sin(degToRad(heading)) * -1.0f;
			float yVec = fivef * cos(degToRad(heading));
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.x += xVec, pos.y += yVec;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}
		if (GetAsyncKeyState(VK_KEY_A) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 266)) {
			float fivef = .5f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading + 0.5f);
		}
		if (GetAsyncKeyState(VK_KEY_D) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 271)) {
			float fivef = .5f;
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading - 0.5f);
		}
		if (GetAsyncKeyState(VK_SHIFT) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, ControlFrontendRb)) {
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.z -= 0.5;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}
		if (GetAsyncKeyState(VK_SPACE) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, ControlFrontendLb)) {
			float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
			ENTITY::SET_ENTITY_HEADING(playerPed, heading);
			pos.z += 0.5;
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
		}



	}


	///////////////////////////protecoes 

	if (featureAnticrash)
	{
		STREAMING::_SET_FOCUS_AREA(9999.0f, 9999.0f, -9999.0f, 0.0f, 0.0f, 0.0f);

		CAM::SET_CAM_ACTIVE(CAM::GET_RENDERING_CAM(), FALSE);
		CAM::SET_CAM_ACTIVE(1, TRUE);
		CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 3000, FALSE, FALSE);


	}

	if (feature15m == TRUE)

	{
		UNK3::_NETWORK_SHOP_BEGIN_SERVICE(&netShop, 1474183246, -1586170317, 1445302971, 15000000, 4);
		if (!UNK3::_NETWORK_SHOP_CHECKOUT_START(netShop)) {

		}
	}
	if (feature10m == TRUE)

	{
		UNK3::_NETWORK_SHOP_BEGIN_SERVICE(&netShop, 1474183246, -1586170317, 1445302971, 10000000, 4);
		if (!UNK3::_NETWORK_SHOP_CHECKOUT_START(netShop)) {

		}
	}


	if (feature10m == TRUE)

	{
		UNK3::_NETWORK_SHOP_BEGIN_SERVICE(&netShop, 1474183246, -1586170317, 1445302971, 5000000, 4);
		if (!UNK3::_NETWORK_SHOP_CHECKOUT_START(netShop)) {

		}
	}





	if (featuremobileopmult)
	{
		GAMEPLAY::SET_FAKE_WANTED_LEVEL(6);
	}
	else if (!featuremobileopmult)
	{
		GAMEPLAY::SET_FAKE_WANTED_LEVEL(0);
	}






	if (featurenoclipSafety)
	{

	}



	//veiculo




    if (DeleteVehicle)
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false))
		{
			int Vehicle = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (!NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Vehicle))
				while (!NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Vehicle));

			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Vehicle, 1, 1);
			VEHICLE::DELETE_VEHICLE(&Vehicle);
		}
		bool DeleteVehicle = false;
	}
	if (!featureVehSeatbelt)
	{

	}
	if (featureVehSeatbelt)
	{

	}

	if (featureVehSlide)
	{

	}
	else
	{

	}
	if (featureVehSuperRPM, PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
	}
	if (!featureVehSuperRPM)
	{


	}
	//money



	//Weapon Options
	if (bPlayerExists && featureWeaponNoReload)
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);

					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
				}
			}
		}



	}
	if (featureWeaponExplosiveAmmo)
	{

		if (bPlayerExists)
			GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(player);


	}




	if (featureTeleportGun)
	{
		if (bPlayerExists && PED::IS_PED_SHOOTING_IN_AREA)
		{
			Vector3 iCoord;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &iCoord))
			{
				ENTITY::SET_ENTITY_COORDS(playerPed, iCoord.x, iCoord.y, iCoord.z + 1, 0, 0, 1, 1);
				WAIT(0);

			}
		}

	}

	if (featureonehit)
	{

		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 999999);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, 999999);


	}

	if (featurerapidfire)
	{


		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_PED_ID(), 1);
			Vector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
			Vector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			Vector3 gameplayCamDirection = RotationToDirection(gameplayCamRot);
			Vector3 startCoords = addVector(gameplayCam, (multiplyVector(gameplayCamDirection, 1.0f)));
			Vector3 endCoords = addVector(startCoords, multiplyVector(gameplayCamDirection, 500.0f));
			Hash weaponhash;
			WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, 1);
			if (CONTROLS::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, playerPed, 1, 1, 0xbf800000);
			}
		}






	}

	if (featurerapidMultiplier)
	{


		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, (MultiplierShot));
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, (MultiplierShot));




	}
#define OFFSET_WEAPON_RECOIL            0x2A4           //float set to 0


	if (featureRecoil)
	{





	}

	if (featureMolotov)

	{
		float startDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		float endDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		startDistance += 0.25;
		endDistance += 1000.0;
		if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING_IN_AREA)
		{
			Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK"); //WEAPON_FIREWORK
			if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
			{
				WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, false);
				while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
					WAIT(0);
			}
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).z, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		}

	}
	if (featureFlare)

	{
		float startDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		float endDistance = distance(&CAM::GET_GAMEPLAY_CAM_COORD(), &ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true));
		startDistance += 0.25;
		endDistance += 1000.0;
		if (PED::IS_PED_ON_FOOT(playerPed))
		{
			Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
			if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
			{
				WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, false);
				while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
					WAIT(0);
			}
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), startDistance)).z, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).x, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).y, add(&CAM::GET_GAMEPLAY_CAM_COORD(), &multiply(&rotDirection(&CAM::GET_GAMEPLAY_CAM_ROT(0)), endDistance)).z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		}

	}

	if (featureWeaponExplosiveMelee)
	{

	}
	if (featureWeaponOneHit)
	{

	}


	if (featureWeaponFirework)
	{

	}
	if (featureFakeBags)
	{

	}




	if (featureWeaponnuke)
	{



	}



	LPCSTR vehModels[283] = {
		"NINEF", "NINEF2", "BLISTA", "ASEA", "ASEA2", "T20", "BRAWLER", "COQUETTE3", "VINDICATOR"
		"COACH", "AIRBUS", "ASTEROPE", "AIRTUG", "AMBULANCE", "BARRACKS", "BARRACKS2", "BALLER", "BALLER2",
		"BJXL", "BANSHEE", "BENSON", "BFINJECTION", "BIFF", "BLAZER", "BLAZER2", "BLAZER3", "BISON", "BISON2",
		"BISON3", "BOXVILLE", "BOXVILLE2", "BOXVILLE3", "BOBCATXL", "BODHI2", "BUCCANEER", "BUFFALO", "BUFFALO2", "BULLDOZER",
		"BULLET", "BURRITO", "BURRITO2", "BURRITO3", "BURRITO4", "BURRITO5", "CAVALCADE", "CAVALCADE2", "POLICET",
		"GBURRITO", "CABLECAR", "CADDY", "CADDY2", "CAMPER", "CARBONIZZARE", "CHEETAH", "COMET2", "COGCABRIO", "COQUETTE",
		"CUTTER", "GRESLEY", "DILETTANTE", "DILETTANTE2", "DUNE", "DUNE2", "HOTKNIFE", "DLOADER", "DUBSTA", "DUBSTA2",
		"DUMP", "RUBBLE", "DOCKTUG", "DOMINATOR", "EMPEROR", "EMPEROR2", "EMPEROR3", "ENTITYXF", "EXEMPLAR", "ELEGY2",
		"F620", "FBI", "FBI2", "FELON", "FELON2", "FELTZER2", "FIRETRUK", "FLATBED", "FORKLIFT", "FQ2",
		"FUSILADE", "FUGITIVE", "FUTO", "GRANGER", "GAUNTLET", "HABANERO", "HAULER", "HANDLER", "INFERNUS", "INGOT",
		"INTRUDER", "ISSI2", "JACKAL", "JOURNEY", "JB700", "KHAMELION", "LANDSTALKER", "LGUARD", "MANANA", "MESA",
		"MESA2", "MESA3", "CRUSADER", "MINIVAN", "MIXER", "MIXER2", "MONROE", "MOWER", "MULE", "MULE2",
		"ORACLE", "ORACLE2", "PACKER", "PATRIOT", "PBUS", "PENUMBRA", "PEYOTE", "PHANTOM", "PHOENIX", "PICADOR",
		"POUNDER", "POLICE", "POLICE4", "POLICE2", "POLICE3", "POLICEOLD1", "POLICEOLD2", "PONY", "PONY2", "PRAIRIE",
		"PRANGER", "PREMIER", "PRIMO", "RANCHERXL", "RANCHERXL2", "RAPIDGT", "RAPIDGT2", "RADI", "RATLOADER",
		"REBEL", "REGINA", "REBEL2", "RENTALBUS", "RUINER", "RUMPO", "RUMPO2", "RHINO", "RIOT", "RIPLEY",
		"ROCOTO", "ROMERO", "SABREGT", "SADLER", "SADLER2", "SANDKING", "SANDKING2", "SCHAFTER2", "SCHWARZER", "SCRAP",
		"SEMINOLE", "SENTINEL", "SENTINEL2", "ZION", "ZION2", "SERRANO", "SHERIFF", "SHERIFF2", "SPEEDO", "SPEEDO2",
		"STANIER", "STINGER", "STINGERGT", "STOCKADE", "STOCKADE3", "STRATUM", "SULTAN", "SUPERD", "SURANO", "SURFER",
		"SURFER2", "SURGE", "TACO", "TAILGATER", "TAXI", "TRASH", "TRACTOR", "TRACTOR2", "TRACTOR3",
		"TIPTRUCK", "TIPTRUCK2", "TORNADO", "TORNADO2", "TORNADO3", "TORNADO4", "TOURBUS", "TOWTRUCK", "TOWTRUCK2",
		"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3", "VOODOO2", "WASHINGTON", "STRETCH", "YOUGA", "ZTYPE", "SANCHEZ", "SANCHEZ2",
		"POLICEB", "AKUMA", "CARBONRS", "HEXER", "NEMESIS", "ADDER", "VOLTIC", "VACCA", "BIFTA",
		"BAGGER", "BATI", "BATI2", "RUFFIAN", "DAEMON", "DOUBLE", "PCJ", "VADER", "VIGERO", "FAGGIO2",
		"PARADISE", "KALAHARI", "JESTER", "TURISMOR", "ALPHA", "HUNTLEY", "THRUST", "MASSACRO",
		"MASSACRO2", "ZENTORNO", "BLADE", "GLENDALE", "PANTO", "PIGALLE", "WARRENER", "RHAPSODY", "DUBSTA3", "MONSTER",
		"SOVEREIGN", "INNOVATION", "HAKUCHOU", "FUROREGT", "COQUETTE2", "BTYPE", "BUFFALO3", "DOMINATOR2", "GAUNTLET2",
		"MARSHALL", "DUKES", "DUKES2", "STALION", "STALION2", "BLISTA2", "BLISTA3", "VIRGO", "WINDSOR",
		"INSURGENT", "INSURGENT2", "TECHNICAL", "KURUMA", "KURUMA2", "JESTER2", "CASCO",
		"GUARDIAN", "ENDURO", "LECTRO", "SLAMVAN", "SLAMVAN2", "RATLOADER2", "FELTZER3", "OSIRIS"
	};


	/*if (featureWepModVehGun)
	{
	FLOAT heading = ENTITY::GET_ENTITY_HEADING(playerPed);
	//





	((char *)vehModels[rand() % 283]);
	Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
	Vector3 dir = rot_to_direction(&rot);
	Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	float spawnDistance = get_distance(&camPosition, &playerPosition);
	spawnDistance += 5;
	Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

	if (bPlayerExists)
	{
	if (PED::IS_PED_SHOOTING(playerPed))
	{
	Vector3 playerOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 5, 0);
	if (PED::IS_PED_ON_FOOT(playerPed))
	{
	//	STREAMING::REQUEST_MODEL(model);
	while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
	{
	Vehicle veh = VEHICLE::CREATE_VEHICLE(model, spawnPosition.x, spawnPosition.y, spawnPosition.z, heading, 1, 1);
	VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 120.0);
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
	}

	}
	}
	}
	}**/
	if (featureSuperPunch)
	{
		GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(PLAYER::PLAYER_PED_ID());
		GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(PLAYER::PLAYER_ID());
	}

	if (featuretrigger)
	{

	}

	if (featureaim)
	{

	}


	if (featureTimePaused)
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePaused = true;
	}
	else
	{
		TIME::PAUSE_CLOCK(featureTimePaused);
		featureTimePaused = false;
	}
	if (featureTimeSynced)
	{
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}



	if (featureSnow)
	{


		GAMEPLAY::_SET_RAIN_FX_INTENSITY(Snow);





	}
	if (featureSnow = FALSE)
	{


		GAMEPLAY::_SET_RAIN_FX_INTENSITY(0);





	}
	//World Options
	if (featureWorldMoonGravity)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(2);
	}
	else
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(0);
	}


	if (featureBlackout)
	{

		GRAPHICS::_SET_BLACKOUT(0);



	}

	if (featurehidhub)
	{
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();

	}
	if (featurewave)
	{




		WATER::_SET_WAVES_INTENSITY(waves);


	}

	if (featureLaunch)

	{
		const int numElements = 10;
		const int arrSize = numElements * 2 + 2;
		Vehicle veh[arrSize];
		veh[0] = numElements;
		int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), veh);
		if (veh != NULL)
		{
			for (int i = 1; i <= count; i++)
			{
				int offsettedID = i;
				if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
				{
					RequestNetworkControl(veh[offsettedID]);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh[offsettedID], 10000.0f);
				}
			}
		}


	}


	
	if (featureMeteor)
	{
	
	}

	if (featureMeteorUpdated)
	{

	


	}
	//featureSpectate

	if (featureSpectate == true)
	{
		AIWanderCar(PLAYER::PLAYER_PED_ID());
		SpectateMode(true, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));
	}
	if (featureSpectate == false)
	{
		AI::CLEAR_PED_TASKS(PLAYER::PLAYER_ID());
		SpectateMode(false, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));

	}

	if (featurefreezeplayer == true)
	{

		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer));



	}


	if (featureShakecam)
	{


		Player playerPed2 = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
		Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed2, 0);
		FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z + 10, 29, 1.0f, false, true, (ShakeCam_));


	}
	if (featureforce)
	{

		Player playerPed2 = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
		Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed2, 0);
		FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 7, 9.0f, false, true, 0.0f);



	}
	if (featureCash25)//0xB8CFE3E3
	{
	


	}

	if (featureCash)

	{
		float angle = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 3.14159265359;
		float radius = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 5.0f;
		float x = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).x + radius * cos(angle);
		float y = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).y + radius * sin(angle);
		STREAMING::REQUEST_MODEL(0x113FD533);
		if (STREAMING::HAS_MODEL_LOADED(0x113FD533))
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, x, y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).z + (Delay), 0, 2000, 0x113FD533, 0, 1);
		}




	}
	if (featureCashfake)

	{
		float angle = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 3.14159265359;
		float radius = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 5.0f;
		float x = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).x + radius * cos(angle);
		float y = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).y + radius * sin(angle);
		STREAMING::REQUEST_MODEL(0x113FD533);
		if (STREAMING::HAS_MODEL_LOADED(0x113FD533))
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, x, y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).z + (Delay), 0, 0, 0x113FD533, 0, 1);
		}




	}

	if (featureCash2)
	{





	}

	if (featureCash3)
	{
		

	}
	if (featureCash4)
	{
		Hash moneycase = $("pickup_money_security_case");
		Hash moneybag = $("prop_alien_egg_01");


		float angle = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 3.14159265359;
		float radius = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 5.0f;
		float x = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).x + radius * cos(angle);
		float y = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).y + radius * sin(angle);

		STREAMING::REQUEST_MODEL(moneybag);
		if (STREAMING::HAS_MODEL_LOADED(moneybag))
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0x14568F28, x, y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).z + 1.0f, 0, 2000, moneybag, 0, 1);
		}

	}
	if (featureCash5)
	{
		Hash moneycase = $("pickup_money_security_case");
		Hash moneybag = $("prop_cash_case_01");


		float angle = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 3.14159265359;
		float radius = GAMEPLAY::GET_RANDOM_INT_IN_RANGE(0, MoneyRange_) * 5.0f;
		float x = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).x + radius * cos(angle);
		float y = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).y + radius * sin(angle);

		STREAMING::REQUEST_MODEL(moneybag);
		if (STREAMING::HAS_MODEL_LOADED(moneybag))
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0x14568F28, x, y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true).z + 1.0f, 0, 2000, moneybag, 0, 1);
		}

	}

	if (featureBurn) {
		featureBurn = true;
		Vector3 Coords2 = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
		FIRE::START_SCRIPT_FIRE(Coords2.x, Coords2.y, Coords2.z, 1, false);

	}
	if (featureBurn == false)

	{



	}

	if (featureAirstrike) {

		Vector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1);
		Coords.z += 15;
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 35, Coords.x, Coords.y, Coords.z, 250, 1, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), 1, 1, 500);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 30, Coords.x, Coords.y, Coords.z, 250, 0, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(Coords.x, Coords.y, Coords.z + 55, Coords.x, Coords.y, Coords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), true, true, 600);







	}

	if (featureAirstrike == false)
	{



	}


	if (featureCrashClone)
	{
	


	}


	if (featureCrashClone2)
	{

		PED::CLONE_PED(selectedPlayer,1,1,1);

	}



	




	
	if (featureFire) {

		featureFire = true;


		Player playerPed2 = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
		Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed2, 0);
		FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 13, 1.0f, true, false, 0.0f);




	}
	if (featureFire == false)

	{



	}

	if (featureFire2 == true)
	{


		Player playerPed2 = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
		Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed2, 0);
		FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 12, 1.0f, true, false, 0.0f);



	}


	if (featureFire2 == false)

	{



	}

	if (featureExplosionl == true)
	{
		Player playerPed2 = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer);
		Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed2, 0);
		FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, (Explosionl_), (Damagel_), true, false, (Shakel_));


	}
	//weather

	if (featurDisable)
	{

		AI::TASK_USE_MOBILE_PHONE_TIMED(playerPed, 999999);




	}


	//allplayers

	
	/*if (featureESP == true)
		void featureESP(Player target);
	{
		if (target != PLAYER::PLAYER_ID() && ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target)))
		{
			Vector3 locationOne = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(target), false);
			Vector3 locationTwo = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
			GRAPHICS::DRAW_LINE(locationOne.x, locationOne.y, locationOne.z, locationTwo.x, locationTwo.y, locationTwo.z, 255, 0, 0, 255);
		}
	}*/
	


	//Misc Options
	if (featureMiscHideHud)
	{
		UI::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
	if (featureMiscMobile)
	{
		if (bPlayerExists)
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
	}
}





void process_main_menu() {

	Menu::SetOptionCount(0);
	//Main Menu

	if (Menu::GetActiveMenu() == "MainMenu") {
		//GRAPHICS::DRAW_SPRITE(pszDictName, pszTextureName, 0.0, 0.0, 200.0f, 200.0f, 0.0f, 255, 255, 255, 255);
		//Menu::Texture("commonmenu", "gradient_bgd", 0.125f, 0.1175f, 0.22f, 0.084f, 0, 255, 255, 255, 255);

		Menu::Title("EV0LUTION");
		Menu::SubTitle("Made by Shaxzy");
		Menu::Submenu("Self", "SelfOptions");//1			
		Menu::Submenu("Protections", "ProtectionsOptions");//2	
		Menu::Submenu("Cash Options", "CashOptions");//3
		Menu::Submenu("Vehicle Spawner", "VehicleOptions");//4
		Menu::Submenu("Vehicle Options", "VehicleOptionsModz");//5
		Menu::Submenu("Weapon", "WeaponOptions");//6
		Menu::Submenu("World", "WorldOptions");//7
		Menu::Submenu("Teleport", "TeleportOptions");//8
		Menu::Submenu("Online Players", "OnlineOptions");//9
		Menu::Submenu("All Online Players", "OnlinePlayersOptions");
		Menu::Submenu("Misc", "Misc");
		if (Menu::Option("Kill Game")) exit(0);
		Menu::AboveMapNotification("Welcome to ~g~ev0lution", 1);



	}
	//Self Menu

	if (Menu::GetActiveMenu() == "SelfOptions") {
	//
		
		
		//Menu::Texture("mpshops", "shopui_title_graphics_sale", 0.125f, 0.1175f, 0.22f, 0.084f, 0, 0, 0, 0, 255);
		//Menu::Texture("shopui_title_lowendfashion", "shopui_title_lowendfashion", 0.125f, 0.1175f, 0.22f, 0.084f, 0, 255, 255, 255, 255);

		Menu::Title("Self");
		Menu::Submenu("Models", "models");
		Menu::Submenu("Animations Options", "AnimationsOptions");
		Menu::Submenu("Animal Animations", "AnimationsAnimal");
		Menu::Submenu("Animation Scenarios", "AnimationScenarios");
		Menu::Submenu("Walk Types", "WalkTypes2");
		Menu::Submenu("Clone self", "Clone");
		//Menu::Submenu("Movement Clipsets", "MovementClipsets");
		Menu::Submenu("Self Info", " SelfInfo");
		Menu::Submenu("Force Powers", "ForcePowers");
		Menu::ToggleOption("GodMode", &featurePlayerInvincible);
		Menu::ToggleOption("NoRagDoll", &featureNoRagDoll);
		if (Menu::Option("Heal Player")) HealPlayer();
		Menu::ToggleOption("Never Wanted", &featurePlayerNeverWanted);
		Menu::ToggleOption("Fast Run", &featurePlayerFastRun);
		Menu::ToggleOption("Ultra Run", &featurePlayerUltraRun);
		Menu::IntOption("~b~Player Run Multiplier:", &Scale, 1, 0, 1000);
		Menu::ToggleOption("Enable Player Run Scale Multiplier", &featurePlayerRunScale);
		Menu::ToggleOption("Super Jump", &featurePlayerSuperJump);
		Menu::ToggleOption("Ultra Jump", &featurePlayerUltraJump);
		Menu::IntOption("Player Ultra Jump Multiplier:", &Multi, 5, 0, 1000);
		Menu::ToggleOption("Superman Fly", &featureMiscFlyMode);
		Menu::ToggleOption("Invisible", &featurePlayerInvisible);
		Menu::ToggleOption("Off Radar", &featurePlayeroffradar);
		Menu::ToggleOption("Slowmotion", &featurePlayerslowmotion);
		if (Menu::Option("Fix Player")) featurefixPlayer();
		Menu::ToggleOption("~r~Forcefield", &featureForcefiel);
		Menu::ToggleOption("Fast Swim", &featurefastswim);
		Menu::IntOption("Player Fast Swim Multiplier:", &Swim, 2, 0, 1000);
		Menu::ToggleOption("Tiny Player", &featurePlayerScale);
		Menu::IntOption("Player Opacity ~b~%:", &Opacy, 10, 0, 1000);
		Menu::ToggleOption("Enable Player Opacity", &featureOpaci);
		Menu::ToggleOption("Walk on Air", &featureWalk);
		/////////////////////
		Menu::ToggleOption("~b~RP Loop", &featureRPLoop);
		Menu::ToggleOption("Free Cam", &featureclip);
		Menu::ToggleOption("No Clip", &featureclip2);







		//Menu::ToggleOption("Mobile Radio", &featuremobileRadio);
		//Menu::ToggleOption("6 Stars Wanted Level", &featuremobileopmult);
		//Menu::ToggleOption("Clone Crash test", &featureCrash);
		//Menu::ToggleOption("Crash cargo", &featurelaghimout);
		//Menu::ToggleOption("Noclip", &featurenoclipSafety);
		//if (Menu::Option("Self Explode")) featureexplodeself();








	}

	if (Menu::GetActiveMenu() == "AnimationsOptions")
	{

		Menu::Title("Animations");
		if (Menu::Option("~r~Stop Animation")) featureStop();
		if (Menu::Option("Sexy Dance")) Sexy();
		if (Menu::Option("Sex Giver")) Sex();
		if (Menu::Option("Stripper Dance")) Stripper();
		if (Menu::Option("Pole Dance")) Pole();
		if (Menu::Option("Push Ups")) Push();
		if (Menu::Option("Sit Ups")) Sit();
		if (Menu::Option("Celebrate")) Celebrate();
		if (Menu::Option("Suicide")) Suicide();
		if (Menu::Option("Showering")) Showering();
		if (Menu::Option("Dog Fucking")) Dog();
		if (Menu::Option("Meditation")) meditation();
		if (Menu::Option("Sex Take")) SexTake();
		if (Menu::Option("On Fire ")) onfire();

	}
	if (Menu::GetActiveMenu() == "AnimationsAnimal")
	{

		Menu::Title("Animal Animations");
		if (Menu::Option("~r~Stop Animation")) featureStop();
		if (Menu::Option("Boar ")) Boar();
		if (Menu::Option("Cat ")) Cat();
		if (Menu::Option("Cow ")) Cow();
		if (Menu::Option("Coyote ")) Coyote();
		if (Menu::Option("Coyote(2) ")) Coyote2();
		if (Menu::Option("Chicken ")) Chicken();
		if (Menu::Option("Deer")) Deer();
		if (Menu::Option("Rottweiler")) Rottweiler();
		if (Menu::Option("Retreiver")) Retreiver();
		if (Menu::Option("Shepard ")) Shepard();
		if (Menu::Option("Shepard ")) Shepard();
		if (Menu::Option("Lion(1)")) Lion1();
		if (Menu::Option("Lion(2)")) Lion2();
		if (Menu::Option("Pig ")) Pig();
	



	}




	if (Menu::GetActiveMenu() == "AnimationScenarios")
	{

		Menu::Title("Scenarios");
		if (Menu::Option("~r~Stop Scenario")) featureStop();
		if (Menu::Option("Musician")) Musician();
		if (Menu::Option("Welding")) Welding();
		if (Menu::Option("Lifting Weights")) Lifting();
		if (Menu::Option("Gardener Leaf Blower")) Gardener();
		if (Menu::Option("Binoculars")) Binoculars();
		if (Menu::Option("Bum Freeway")) Bum();
		if (Menu::Option("High Class Prostitute")) High();
		if (Menu::Option("Smoke Break"))Smoke();
		if (Menu::Option("Hammering")) Hammering();
		if (Menu::Option("Maid Clean")) Maid();
		if (Menu::Option("Paparazzi ")) Paparazzi();
		if (Menu::Option("Strip Watch")) Strip();
		if (Menu::Option("Partying")) Partying();
		if (Menu::Option("Smoking Pot")) Smoking();
		if (Menu::Option("Stupor")) Stupor();
		if (Menu::Option("Sumbathe")) Sumbathe();
		if (Menu::Option("Tourist Mobile")) Tourist();
		if (Menu::Option("Statue")) Statue();








	}
	if (Menu::GetActiveMenu() == "ForcePowers")
	{


		Menu::Title("Force Powers");
		if (Menu::Option("Force Explode Cars")) Force1();
		if (Menu::Option("Force Push Cars")) Force2();
		if (Menu::Option("Force Persuade - Dance")) Force3();
		if (Menu::Option("Force Persuade - Riot")) Force4();
		if (Menu::Option("Force Persuade - Follow Me")) Force5();
		if (Menu::Option("Force Persuade - Push-Ups")) Force6();
		if (Menu::Option("Force Burn Peds")) Force7();
		if (Menu::Option("Force Throw Peds")) Force8();
		if (Menu::Option("Force Push Peds")) Force9();
		if (Menu::Option("Force Launch Cars")) Force10();
		/*	if (Menu::Option("Kill Attackers")) Force11();
		if (Menu::Option("Force Jump")) Statue();

		*/



	}
	if (Menu::GetActiveMenu() == "SelfInfo")
	{
		Menu::Title(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()));






	}
	if (Menu::GetActiveMenu() == "WalkTypes2")
	{
		Menu::Title("Walk Types");
		if (Menu::Option("~r~Reset")) featureStop();
		if (Menu::Option("Default Male")) Male();
		if (Menu::Option(" Deafault Female")) Female();
		if (Menu::Option("Drunk ")) Drunk();
		if (Menu::Option("Fatass ")) Fatass();
		if (Menu::Option(" Business")) Business();
		if (Menu::Option("Hiker ")) Hiker();
		if (Menu::Option("Running ")) Running();
		if (Menu::Option("Limp ")) Limp();

	

	}

	if (Menu::GetActiveMenu() == "Clone")
	{
		Menu::Title("Clones");
		if (Menu::Option("Clone Pole Dancing")) Pole2();
		if (Menu::Option("Clone Push")) Meditation();
		if (Menu::Option("Clone Dance ")) Dance2();
		if (Menu::Option("Clone Piggyback")) Teste2();
		if (Menu::Option("Clone Celebrate ")) Teste3();
		if (Menu::Option("Clone Suicide ")) t4();
		if (Menu::Option("Clone Showering ")) t5();
		if (Menu::Option("Clone Meditation ")) t6();
		if (Menu::Option("Clone SexTake ")) t7();
		if (Menu::Option("Clone on Fire ")) t8();
		if (Menu::Option("Clone Idk ")) t9();
		if (Menu::Option("Clone Sad ")) t10();
		if (Menu::Option("Clone Dead ")) t11();
		if (Menu::Option("Clone Prostitute ")) t12();
		if (Menu::Option("Clone Running away ")) t13();
		if (Menu::Option("Clone Dizzy ")) t14();
		if (Menu::Option("Clone Dance (2) ")) t15();
	


	}

	
	

	
	//protecao


	if (Menu::GetActiveMenu() == "ProtectionsOptions")

	{
		Menu::Title("Protections");
		Menu::ToggleOption("Anti Crash Cam", &featureAnticrash);


	}

	//cash


	if (Menu::GetActiveMenu() == "CashOptions")

	{
		Menu::Title("Cash Options");
		Menu::Submenu("Stealth ~g~$ Money $ ~w~Method", "Stealth");





	}

	if (Menu::GetActiveMenu() == "Stealth") {

		Menu::Title("Stealth Options");
		Menu::ToggleOption("~g~15 MI Loop", &feature15m);
		Menu::ToggleOption("~g~10 MI Loop", &feature10m);
		Menu::ToggleOption("~g~5 MI Loop", &feature5m);



	}







	//Vehicle Menu / Submenu
	if (Menu::GetActiveMenu() == "VehicleOptions") {

		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Menu::Title("Vehicle");
		Menu::Submenu("Vehicle Spawner", "CarSpawn");

	}
	if (Menu::GetActiveMenu() == "CarSpawn")
	{
		Menu::Title("Spawner");
		Menu::Submenu("Compacts", "Compacts");
		Menu::Submenu("Super", "Super");
		Menu::Submenu("Sport", "Sport");
		Menu::Submenu("Off-Road", "Off-Road");
		//Menu::Submenu()

	}
	if (Menu::GetActiveMenu() == "Compacts")
	{
		Menu::Title("Compacts");
		if (Menu::Option("Blista")) SpawnVehicle(0xEB70965F);
		if (Menu::Option("Panto")) SpawnVehicle(0xE644E480);
		if (Menu::Option("Dilettante")) SpawnVehicle(0xBC993509);
		if (Menu::Option("Issi")) SpawnVehicle(0xB9CB3B69);
		if (Menu::Option("Rhapsody")) SpawnVehicle(0x322CF98F);
		if (Menu::Option("Brioso(b)")) SpawnVehicle(0x5C55CB39);

	}
	if (Menu::GetActiveMenu() == "Super")
	{
		Menu::Title("Super");
		if (Menu::Option("Adder")) SpawnVehicle(0xb779a091);
		if (Menu::Option("Banshee 900R")) SpawnVehicle(0x25c5af13);
		if (Menu::Option("Sultan RS")) SpawnVehicle(0xee6024bc);
		if (Menu::Option("T20")) SpawnVehicle(0x6322b39a);
		if (Menu::Option("Visione(b)")) SpawnVehicle(0xc4810400);
		if (Menu::Option("Nero Custom(b)")) SpawnVehicle(0x4131f378);
		if (Menu::Option("FMJ")) SpawnVehicle(0x5502626c);
		if (Menu::Option("GP1(b)")) SpawnVehicle(0x4992196c);
		if (Menu::Option("Itali GTB(b)")) SpawnVehicle(0x85e8e76b);
		if (Menu::Option("Itali GTB Custom(b)")) SpawnVehicle(0xe33a477b);
		if (Menu::Option("Rocket Voltic(b)")) SpawnVehicle(0x3af76f4a);
		if (Menu::Option("Zentorno")) SpawnVehicle(0xac5df515);


	}
	if (Menu::GetActiveMenu() == "Sport")
	{
		Menu::Title("Sport");
		if (Menu::Option("9F")) SpawnVehicle(0x3d8fa25c);
		if (Menu::Option("9F Cabrio")) SpawnVehicle(0xa8e38b01);
		if (Menu::Option("Banshee")) SpawnVehicle(0xc1e908d2);
		if (Menu::Option("Futo")) SpawnVehicle(0x7836ce2f);
		if (Menu::Option("Jester")) SpawnVehicle(0xb2a716a3);
		if (Menu::Option("Jester (Racecar)")) SpawnVehicle(0xbe0e6126);
		if (Menu::Option("Jester Classic")) SpawnVehicle(0xf330cb6a);
		if (Menu::Option("Kuruma")) SpawnVehicle(0xae2bfe94);
		if (Menu::Option("Kuruma (Armored)")) SpawnVehicle(0x187d938d);
		if (Menu::Option("Neon(b)")) SpawnVehicle(0x91ca96ee);
		if (Menu::Option("Sultan")) SpawnVehicle(0x39da2754);
	}
	if (Menu::GetActiveMenu() == "Off-Road")
	{
		Menu::Title("Off-Road");
		if (Menu::Option("Mesa")) SpawnVehicle(0x84f42e51);
		if (Menu::Option("Insurgent")) SpawnVehicle(0x7b7e56f0);
		if (Menu::Option("Insurgent Pick-Up")) SpawnVehicle(0x9114eada);
		if (Menu::Option("Liberator")) SpawnVehicle(0xcd93a7db);
		if (Menu::Option("Marshall")) SpawnVehicle(0x49863e9c);
		if (Menu::Option("Space Docker")) SpawnVehicle(0x1fd824af);
		if (Menu::Option("Romero Hearse")) SpawnVehicle(0x2560b2fc);
		if (Menu::Option("Ramp Buggy(b)")) SpawnVehicle(0xceb28249);
		if (Menu::Option("Trophy Truck(b)")) SpawnVehicle(0x612f4b6);
		


	}


		



	//Vehicles Options



	if (Menu::GetActiveMenu() == "VehicleOptionsModz")

	{
		Menu::Title("Vehicle Options");
		if (Menu::Option("Fix Vehicle")) featurefixPlayer();
		//if (Menu::Option("Fix Vehicle")) FiXVehicle;
		if (Menu::Option("Flip Vehicle")) FlipCar;
		if (Menu::Option("Paint Random")) PaintRandom;
		if (Menu::Option("Delete Vehicle")) DeleteVehicle;
		if (Menu::Option("Max Vehicle")) maxvehicle;



	}














	//Weapons Menu / Submenu
	//DRAW_TEXTURE("shopui_title_carmod2", "shopui_title_carmod2", titlebox, 0.1175f, 0.23f, 0.083f, 0, 255, 255, 255, 255);

	if (Menu::GetActiveMenu() == "WeaponOptions") {

		Menu::Title("Weapon");
		if (Menu::Option("Give All Weapons")) giveweap();
		if (Menu::Option("~o~Give All MK2 Weapons")) giveweapmk2();
		Menu::Submenu("~b~Vehicles Shoot", "Shoot");
		Menu::Submenu("Weapons Modz", "Modz");
		Menu::ToggleOption("Unlimited Ammo", &featureWeaponNoReload);
		Menu::ToggleOption("Teleport Gun", &featureTeleportGun);
		Menu::ToggleOption("One hit kill", &featureonehit);
		Menu::ToggleOption("Rapid Firer", &featurerapidfire);
		Menu::ToggleOption("Weapon Damage Multiplier ", &featurerapidMultiplier);
		Menu::IntOption("~b~Damage Multiplicator", &MultiplierShot, 5, 1, 1000);
		Menu::ToggleOption("Weapon No Recoil ", &featureRecoil);






		
		Menu::ToggleOption("molotov", &featureWepModMolotov);
		Menu::ToggleOption("Firework", &featureWeaponFirework);
		Menu::ToggleOption("Fake money bag", &featureFakeBags);
		Menu::ToggleOption("Rapid Fire", &featureWeaponnuke);
		Menu::ToggleOption("Car", &featureWepModVehGun);
		Menu::ToggleOption("Super Punch", &featureSuperPunch);
		Menu::ToggleOption("TriggerBot", &featuretrigger);
		Menu::ToggleOption("TriggerBot", &featureaim);

		//test 123 1337
		


	}
	if (Menu::GetActiveMenu() == "Shoot") {

		Menu::Title("Vehicles Shoot");





	}


	if (Menu::GetActiveMenu() == "Modz") {
		Menu::Title("Weapons Modz");
		Menu::ToggleOption("Molotov", &featureMolotov);
		Menu::ToggleOption("Flare", &featureFlare);



	}









	//World Menu / Submenus
	if (Menu::GetActiveMenu() == "WorldOptions") {

		Menu::Title("World");
		Menu::Submenu("Time", "Time");
		Menu::Submenu("Weather", "Weather");
		Menu::Submenu("World Cars", "WorldCars");
		Menu::ToggleOption("Moon Gravity", &featureWorldMoonGravity);
		Menu::ToggleOption("Blackout", &featureBlackout); 
		Menu::ToggleOption("Hide HUD", &featurehidhub);
		Menu::IntOption("~b~Waves Intesity :", &waves, 1, 0, 1000);
		Menu::ToggleOption("Enable wave intensity", &featurewave);
		if (Menu::Option("Reset Wave Intensity")) reset();
		Menu::ToggleOption("~r~Meteor ~b~Rain", &featureMeteor);


	}


	if (Menu::GetActiveMenu() == "WorldCars") 
	{
		Menu::Title("WorldCars");
		Menu::ToggleOption("Launch All Cars ", &featureLaunch);
	//	Menu::ToggleOption("Explode All Cars ", &featureExplodeCar);
	





	}






	if (Menu::GetActiveMenu() == "Time") {

		Menu::Title("Time");
		if (Menu::Option("Hour Forward")) HourForw();
		if (Menu::Option("Hour Backward")) HourBack();
		Menu::ToggleOption("Freeze Time", &featureTimePaused);
		Menu::ToggleOption("Sync With System", &featureTimeSynced);

	}
	if (Menu::GetActiveMenu() == "Weather") {


		Menu::Title("Weather");
		if (Menu::Option("Extra Sunny")) ExtraSunny();
		if (Menu::Option("Sunny")) Sunny();
		if (Menu::Option("Cloudy")) Cloudy();
		if (Menu::Option("Smoggy")) Smoggy();
		if (Menu::Option("Foggy")) Foggy();
		if (Menu::Option("Stormy")) Stormy();
		if (Menu::Option("Overcast")) overcast();
		if (Menu::Option("Light Snow")) snowlight();
		if (Menu::Option("Snow")) snow();
		if (Menu::Option("Blizzard")) blizzard();
		Menu::IntOption("~b~Rain Pfx  Intesity :", &waves, 1, 0, 1000);
		Menu::ToggleOption("Enable Rain Pfx  Intesity", &featureSnow);







	}


	//Teleport Menu / Submenu
	if (Menu::GetActiveMenu() == "TeleportOptions") {

		Menu::Title("Teleport");
		if (Menu::Option("Waypoint")) teleport_to_marker();



	}



	//Online Menu / Submenu
	if (Menu::GetActiveMenu() == "OnlineOptions")
	{
		Menu::Title("Online");




		for (int i = 0; i < 32; i++)

			if (NETWORK::NETWORK_IS_PLAYER_CONNECTED(i))
				//(ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))

			{

				Menu::Option((ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)), PLAYER::GET_PLAYER_NAME(i)));
			}
		if (OptionPressed && PLAYER::IS_PLAYER_PLAYING(CurrentOption - 1))
		{
			selectedPlayer = CurrentOption - 1;
			Menu::ChangeSubmenu("Selected");
			

		}






		/*	Menu::Submenu("Players List", "Players");
		for (int i = 0; i < 32; i++)
		if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)))
		if (NETWORK::NETWORK_IS_PLAYER_CONNECTED(i))
		Menu::PlayerOption(PLAYER::GET_PLAYER_NAME(i), i, &selectedPlayer, &PlayerNames[i]); Menu::ChangeSubmenu("Selected");



		/**		if (OptionPressed && PLAYER::IS_PLAYER_PLAYING(CurrentOption - 1))
		{
		selectedPlayer = CurrentOption - 1;

		Menu::ChangeSubmenu("Selected");

		//	Menu::ChangeSubmenu("Selected");
		}
		*/
	}

	if (Menu::GetActiveMenu() == "Selected")
	{
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), false);
		GRAPHICS::DRAW_MARKER(2, coords.x, coords.y, coords.z + 1.3f, 0, 0, 0, 0, 180, 0, 0.3, 0.3, 0.3, 255, 0, 0, 200, 1, 1, 1, 0, 0, 0, 0);


		Menu::Title(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		Menu::ToggleOption("Spectate", &featureSpectate);
		if (Menu::Option("Teleport To Player ")) Tptoplayer();
		if (Menu::Option("Teleport To Player Car")) TptoplayerCar();

		Menu::ToggleOption("Freeze Player", &featurefreezeplayer);
		//if (Menu::Option("Teleport To Player Car")) TptoplayerCar();
		Menu::Submenu("Troll Options", "TrollPlayer");
		Menu::Submenu("Attach  Options", "AttachOptions");
		Menu::Submenu("Cash  Options", "CashCash");
		if (Menu::Option("Give All Weapons")) GiveAllWeapons();
		if (Menu::Option("Take All Weapons")) TakeAllWeapons();
		if (Menu::Option("Kick Player(Host)")) KickP();
		if (Menu::Option("~r~Blame Player")) Blame();
		if (Menu::Option("Frame Player(kill Everyone)")) frame();
	


		

		
		

	
			if (Menu::Option("Clone Player ")) pclone();




		//	Menu::Submenu("Carsh  Options", "Carsh Options");

		//::CLONE_PED(selectedPlayer, ENTITY::GET_ENTITY_HEADING(playerPed), 1, 1);





	}



	if (Menu::GetActiveMenu() == "TrollPlayer")
	{
		

		Menu::Title("Troll Options");
		Menu::Submenu("~r~Explosion Options", "ExplosionOp");
		Menu::ToggleOption("Shake Cam", &featureShakecam);
		Menu::IntOption("Shake Cam Intensity ~g~N* :", &ShakeCam_, 1, 50, 1000);
		if (Menu::Option("~r~Kill")) Killlayer();
		if (Menu::Option("~g~Silent Kill")) SeilentKilllayer();
		Menu::ToggleOption("Forcefield", &featureforce);
		Menu::ToggleOption("Burn", &featureBurn);
		if (Menu::Option("Kill by Airstrike")) AirstrikeKilllayer();
		Menu::ToggleOption("~y~Kill by Airstrike Loop ", &featureAirstrike);
		Menu::ToggleOption("~r~Crash By Clone", &featureCrashClone);
		if (Menu::Option("Clone Piggyback")) PedsRiot();

		/*
		Menu::ToggleOption("~r~Crash By Clone V2", &featureCrashClone2);
		*/


		//(char* option, int* var, int changeBy, int minimumValue, int maximumValue) {
		//if (Menu::Option("~r~Blame Player")) AddBlamer();	



	}





	if (Menu::GetActiveMenu() == "ExplosionOp")

	{

		Menu::Title("Explosion Options");
		Menu::Submenu("Explosions Types List", "ExplosionsTp");
		Menu::Submenu("Explosions Loop List", "ExplosionsLp");

	}



	//so 1


	if (Menu::GetActiveMenu() == "ExplosionsTp")
	{

		Menu::Title("Explosions Type");
		Menu::Submenu("Explosions List", "ListExp");
		Menu::IntOption("Explosion Type ~g~N* :", &Explosion_, 1, 0, 38);
		Menu::IntOption("Explosion ~r~Damage ~g~N* :", &Damage_, 1, 0, 1000);
		Menu::IntOption("Explosion ~r~Intensity(Shake Cam) ~g~N* :", &Shake_, 1, 0, 1000);
		if (Menu::Option("~r~Explode Player ")) featureloop();






	}


	if (Menu::GetActiveMenu() == "ListExp")
	{
		Menu::Title("Explosions Typelist");
		if (Menu::Option("Grenade")) kill0();
		if (Menu::Option("GrenadeL")) kill1();
		if (Menu::Option("StickyBomb")) kill2();
		if (Menu::Option("Molotov")) kill3();
		if (Menu::Option("Rocket")) kill4();
		if (Menu::Option("Tank Shell")) kill5();
		if (Menu::Option("Hi Octane")) kill6();
		if (Menu::Option("Car")) kill7();
		if (Menu::Option("Plane")) kill8();
		if (Menu::Option("Petrol Pump")) kill9();
		if (Menu::Option("Bike")) kill10();
		if (Menu::Option("Steam")) kill11();
		if (Menu::Option("Flame")) kill12();
		if (Menu::Option("WaterHydrant")) kill13();
		if (Menu::Option("Gas Canister")) kill14();
		if (Menu::Option("Boat")) kill15();
		if (Menu::Option("Ship Destroy")) kill16();
		if (Menu::Option("Truck")) kill17();
		if (Menu::Option("Bullet")) kill18();
		if (Menu::Option("SmokeGL")) kill19();
		if (Menu::Option("SmokeG")) kill20();
		if (Menu::Option("BZGas")) kill21();
		if (Menu::Option("Flare")) kill22();
		if (Menu::Option("Gas Canister2")) kill23();
		if (Menu::Option("Extinguisher")) kill24();
		if (Menu::Option("ProgramAR")) kill25();
		if (Menu::Option("Train")) kill26();
		if (Menu::Option("Barrel")) kill27();
		if (Menu::Option("Propane")) kill28();
		if (Menu::Option("Blimp")) kill29();
		if (Menu::Option("FlameExplode")) kill30();
		if (Menu::Option("Tanker")) kill31();
		if (Menu::Option("Plane Rocket")) kill32();
		if (Menu::Option("Vehicle Bullet")) kill33();
		if (Menu::Option("Gas Tank")) kill34();
		if (Menu::Option("FireWork")) kill35();
		if (Menu::Option("SnowBall ")) kill36();
		if (Menu::Option("ProxMine")) kill37();
		if (Menu::Option("Valkyrie")) kill38();




	}

	//lop
	if (Menu::GetActiveMenu() == "ExplosionsLp")
	{
		
		Menu::Title("Explosions Loop");
		Menu::IntOption("Explosion Loop ~g~N* : ", &Explosionl_, 1, 0, 38);
		Menu::IntOption("Explosion ~r~Damage ~g~N* : ", &Damagel_, 1, 0, 1000);
		Menu::IntOption("Explosion Loop ~r~Intensity(Shake Cam) ~g~N* :", &Shakel_, 1, 0, 1000);
		Menu::ToggleOption("~r~Explode Player Loop", &featureExplosionl);


	}


	if (Menu::GetActiveMenu() == "AttachOptions")
	{

		Menu::Title("Attach Options");
		Menu::ToggleOption("Water Attack", &featureFire);
		Menu::ToggleOption("Flame Attack", &featureFire2);






	}



	if (Menu::GetActiveMenu() == "CashCash")
	{
	

		Menu::Title("~g~$$ ~w~ Money Drop ~g~$$");
		Menu::SubTitle("~r~Do not save the money in the bank.");
		Menu::Submenu("Cash Options", "OptionsC");
		Menu::ToggleOption("~w~Drop ~r~2.5K ~w~Moneybag", &featureCash25);
		Menu::ToggleOption("~w~Drop 2K Moneybag", &featureCash);
		Menu::ToggleOption("~w~Drop Fake Money", &featureCashfake);
		Menu::Submenu(" Prop Drop Types", "PropDrop");


	}
	/*static bool Option(char* option);
	static bool OptionBrk(char* option);
	static bool ToggleOption(char* option, bool *toggle);
	static bool IntOption(char* option, int* var, int changeBy, int minimumValue, int maximumValue);
	(char *option, int *var, int min, int max)
	*/

	char *number[2] = { "pickup_money_security_case", "0x113FD533 " };
	int numbers = 0;


	if (Menu::GetActiveMenu() == "PropDrop")
	{
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(selectedPlayer), false);
		GRAPHICS::DRAW_MARKER(2, coords.x, coords.y, coords.z + 1.3f, 0, 0, 0, 0, 180, 0, 0.3, 0.3, 0.3, 255, 0, 0, 200, 1, 1, 1, 0, 0, 0, 0);

		Menu::Title("~g~$$ ~w~Prop Drop ~g~$$");




		/*	Menu::ToggleOption("~g~Drop 2K Alien Egg", &featureCash2);
		Menu::ToggleOption("~g~Drop 2K Alien Egg", &featureCash3);
		Menu::ToggleOption("~g~Drop 2K Alien Egg", &featureCash4);
		Menu::ToggleOption("~g~Drop 2K Alien Egg", &featureCash5);

		*/

	}
	if (Menu::GetActiveMenu() == "OptionsC")
	{
		Menu::Title("~g~$$ ~w~Money Options ~g~$$");
		Menu::IntOption("Drop Height", &Delay, 1, 0, 1000);
		Menu::IntOption("Money Drop Radius", &Radius, 1, 0, 1000);
	}









	if (Menu::GetActiveMenu() == "OnlinePlayersOptions")
	{
		Menu::Title("All Players");
		Menu::Submenu("Troll Options", "TrollOptionAll");
		Menu::Submenu("Attack Options", "AttackOptionsAll");


	}







	if (Menu::GetActiveMenu() == "TrollOptionAll")
	{
		Menu::Title("Troll Options");
	
		

	




	}






	if (Menu::GetActiveMenu() == "AttackOptionsAll")
	{
		Menu::Title("Attack Options");

	}






	//// misc

	if (Menu::GetActiveMenu() == "Misc") {

		Menu::Title("Misc");
		Menu::Submenu("Theme", "Themeoptions");
		if (Menu::Option("Broken Screen Phone")) aaaa();
		if (Menu::Option("Disable Phone")) destruir();
		Menu::ToggleOption("Disable Phone", &featurDisable);
		Menu::ToggleOption("Aim(b)", &featureaim);
		//Menu::ToggleOption("", &feature)


		Menu::Submenu("Clear Area", "Areacc");


	}
	if (Menu::GetActiveMenu() == "Themeoptions")
	{

		Menu::Title("Theme Options");
		Menu::Submenu("Title Back", "Theme");
		Menu::Submenu("Title Text", "ThemeText");
		Menu::Submenu("Background", "Background");
		Menu::Submenu("Options ", "ThemeOptions");	
		Menu::Submenu("Scroller ", "Scroller");

	


		//	if (Menu::Option("Save Theme")) Menu::SaveMenuTheme(menuStyleLocation);
		//if (Menu::Option("Load Theme")) Menu::LoadMenuTheme(menuStyleLocation);


	}

		if (Menu::GetActiveMenu() == "Theme")
	{
			Menu::Title("Title Color Options");
			Menu::IntOption("~r~Red:", &TitleBoxRed, 1, 0, 255);
			Menu::IntOption("~g~Green:", &TitleBoxGreen, 1, 0, 255);
			Menu::IntOption("~b~Blue:", &TitleBoxBlue, 1, 0, 255);
			Menu::IntOption("Box ALPHA:", &TitleBoxALPHA, 1, 0, 255);
		


	}

		if (Menu::GetActiveMenu() == "ThemeText")
		{
			Menu::Title("Title Text");
			Menu::IntOption("~r~Red:", &TitleTextRed, 1, 0, 255);
			Menu::IntOption("~g~Green:", &TitleTextGreen, 1, 0, 255);
			Menu::IntOption("~b~Blue:", &TitleTextBlue, 1, 0, 255);
			Menu::IntOption("Box ALPHA:", &TitleTextALPHA, 1, 0, 255);
			Menu::IntOption("Title Font :", &TitleFont, 1, 0, 7);
			Menu::IntOption("SubTitle Font :", &SubTitleFont, 1, 0, 7);



		}

		if (Menu::GetActiveMenu() == "Background")
		{
			Menu::Title("Background");
			Menu::IntOption("~r~Red:", &BackgroundRed, 1, 0, 255);
			Menu::IntOption("~g~Green:", &BackgroundGreen, 1, 0, 255);
			Menu::IntOption("~b~Blue:", &BackgroundBlue, 1, 0, 255);
			Menu::IntOption("Box ALPHA:", &BackgroundALPHA, 1, 0, 255);
		

		}

		if (Menu::GetActiveMenu() == "ThemeOptions")
		{
			Menu::Title("Options");
			Menu::IntOption("~r~Red:", &OptionsRed, 1, 0, 255);
			Menu::IntOption("~g~Green:", &OptionsGreen, 1, 0, 255);
			Menu::IntOption("~b~Blue:", &OptionsBlue, 1, 0, 255);
			Menu::IntOption("Box ALPHA:", &OptionsALPHA, 1, 0, 255);
			Menu::IntOption("Font :", &OptionsFont, 1, 0, 6);

		}
		if (Menu::GetActiveMenu() == "Scroller")
		{
			Menu::Title("Scroller");
			Menu::IntOption("~r~Red:", &ScrollerRed, 1, 0, 255);
			Menu::IntOption("~g~Green:", &ScrollerGreen, 1, 0, 255);
			Menu::IntOption("~b~Blue:", &ScrollerBlue, 1, 0, 255);
			Menu::IntOption("Box ALPHA:", &ScrollerALPHA, 1, 0, 255);


		}





		

	Menu::EndMenu();
}
void reset_globals()
{
	//self
	featurePlayerInvincible = //god	1
		featureNoRagDoll =//Ragdool 2
		featureNoRagDollUpdated =
		featurePlayerNeverWanted = //no cops //4
		featurePlayerFastRun = //fast run //5
		featurePlayerUltraRun = // extreme run //6
		featurePlayerRunScale =
		featurePlayerSuperJump = //superjump // 7
		featurePlayerUltraJump = //Ultrajump //8
		featureMiscFlyMode = //supemanf //9
		featurePlayerInvisible = //Invisi //10
		featurePlayeroffradar =//off //11
		featurePlayerslowmotion =//nonoise //12
		featurePlayerslowmotionUpdated = //12
		featureForcefiel = //14
		featurefastswim =//15
		featurefastswimUpdated = //15
		featurePlayerScale =
		featurePlayerScaleUpdated =
		featureOpaci =
		featureWalk =
		featureRPLoop = 
		featureclip =
		featureclip2 =
		featureAnticrash =
		feature99m =
		feature15m =
		feature10m =
		feature5m =
		featureVehWrapInSpawned =//car
		featureVehSuperRPM =
		featureWeaponExplosiveAmmo =
		featureWepModMolotov =
		featurePlayermoneyDrop =
		featuremobileopmult =
		featurenoclipSafety =
		///////////////weapo
		featureWeaponExplosiveMelee =
		featureTeleportGun =
		featureonehit =
		featurerapidfire =
		featurerapidMultiplier =
		featureRecoil =
		featureMolotov =
		featureFlare =
		featureVehInvincible =
		featureVehInvincibleUpdated =
		DeleteVehicle =
		maxvehicle =
		featureVehSlide =
		featureVehScrollingText =
		featureVehSeatbelt =
		featureWeaponRainbowTint =
		featureVehSeatbeltUpdated =
		featureVehSpeedBoost =
		featureWorldMoonGravity =
		featureBlackout =
		featurehidhub =
		featurewave =
		featureLaunch =
		featureMeteor =
		featureMeteorUpdated =
		//on players
		featureSpectate =
		featureSpectateUpdate =
		featurefreezeplayer =
		featureShakecam =
		featureShakecamUpdated =
		featureCash25 =
		featureCash =
		featureCashfake =
		featureCash2 =
		featureCash3 =
		featureCash4 =
		featureCash5 =
		featureforce =
		featureBurn =
		featureAirstrike =
		featureCrashClone =
		featureCrashClone2 =
		featureFire =
		featureFire2 =
		featureESP =
		featureTimePausedUpdated =
		featureTimePaused =
		featureTimeSynced =
		featureSnow =
		featureMiscLockRadio =
		featurDisable = 
		featureMiscHideHud = false;
	featureMiscMobile = false;
	stealth15MLoopStarted = false;
	MoneyDrop = false;
	featureFakeBags = false;
	featureRhinoGun = false;
	featurePantoGun = false;
	featureExplosionl = false;

}
void main() {



	while (true) {

		Menu::MenuControls();

		if (Menu::HasBeenOpened()) {
			update_features();

			if (Menu::IsMenuOpen()) {
				process_main_menu();
			}
		}
		WAIT(0);
	}
}




void ScriptMain()
{

	srand(GetTickCount());
	main();
}


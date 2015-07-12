//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Achievements for DA
//		these achievements are triggered when the server fires relevant events
//
//=============================================================================



//#ifdef GAME_DLL


#include "cbase.h"

#include "baseachievement.h"
#include "achievementmgr.h"
#include "da_player_shared.h"
#include "c_da_player.h"
#include "../steam/isteamuserstats.h"
#include "../steam/steam_api.h"
#include "steamLeaderboards.h"
#include "igameevents.h"
#include "GameEventListener.h"

CAchievementMgr* g_pAchivementManager = new CAchievementMgr();

CSteamLeaderboards* g_pSteamLeaderboards = new CSteamLeaderboards();


// GET A DIVEPUNCH KILL
class CAchievementDAdivepunch1 : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGoal(1);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("DIVEPUNCHKILL");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{
		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "DIVEPUNCHKILL") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data

			// if the atackers userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				IncrementCount(); // WE ALL GOOD!
			}
		}
	}
};

#define DIVEPUNCHKILL 4 // the stat ID and name from steamworks - not the achievement ID
DECLARE_ACHIEVEMENT(CAchievementDAdivepunch1, DIVEPUNCHKILL, "DIVEPUNCHKILL", 1)



// GET 250 DIVEPUNCH KILLS
class CAchievementDAdivepunch250 : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGoal(250);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("DIVEPUNCHKILL_250");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{
		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "DIVEPUNCHKILL_250") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data

			// if the atackers userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				IncrementCount(); // WE ALL GOOD!
			}
		}
	}
};
#define DIVEPUNCHKILL_250 7
DECLARE_ACHIEVEMENT(CAchievementDAdivepunch250, DIVEPUNCHKILL_250, "DIVEPUNCHKILL_250", 1)


// GET like a bajillion DIVEPUNCH KILLS
class CAchievementDAdivepunchBajillion : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL);
		SetGoal(651438);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("DIVEPUNCHKILL_BAJILLION");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{
		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "DIVEPUNCHKILL_BAJILLION") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data
			DevMsg("DIVEPUNCHKILL_BAJILLION activate");
			// if the atackers userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				IncrementCount(); // WE ALL GOOD!
			}
		}
	}
};
#define DIVEPUNCHKILL_BAJILLION 8
DECLARE_ACHIEVEMENT(CAchievementDAdivepunchBajillion, DIVEPUNCHKILL_BAJILLION, "DIVEPUNCHKILL_BAJILLION", 1)






// Dive away from an explosion for reduced damage
class CAchievementDAdiveaweayfromexplosion : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags( ACH_SAVE_GLOBAL);
		SetGoal(1);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("DIVEAWAYFROMEXPLOSION");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{
		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "DIVEAWAYFROMEXPLOSION") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data
			// if the userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				IncrementCount(); // WE ALL GOOD!
			}
		}
	}
};
#define DIVEAWAYFROMEXPLOSION 10
DECLARE_ACHIEVEMENT(CAchievementDAdiveaweayfromexplosion, DIVEAWAYFROMEXPLOSION, "DIVEAWAYFROMEXPLOSION", 1)







// Dive away from an explosion for reduced damage 250 times
class CAchievementDAdiveaweayfromexplosion250 : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_SAVE_GLOBAL);
		SetGoal(250);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("DIVEAWAYFROMEXPLOSION_250");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{
		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "DIVEAWAYFROMEXPLOSION_250") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data
			// if the userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				IncrementCount(); // WE ALL GOOD!
			}
		}
	}
};
#define DIVEAWAYFROMEXPLOSION_250 5
DECLARE_ACHIEVEMENT(CAchievementDAdiveaweayfromexplosion250, DIVEAWAYFROMEXPLOSION_250, "DIVEAWAYFROMEXPLOSION_250", 1)






// Dive away from an explosion for reduced damage 250 times
class CAchievementDAdodgethis : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_SAVE_GLOBAL);
		SetGoal(1);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("DODGETHIS");
		ListenForGameEvent("event_DA_LEADERBOARD");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{
		if (0 == Q_strcmp(event->GetName(), "event_DA_LEADERBOARD") && C_BasePlayer::GetLocalPlayer())
			DevMsg("leaderboard event recieved by da_achievements");

		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "DODGETHIS") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data
			// if the userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				IncrementCount(); // WE ALL GOOD!
			}
		}
	}
};
#define DODGETHIS 11
DECLARE_ACHIEVEMENT(CAchievementDAdodgethis, DODGETHIS, "DODGETHIS", 1)











/*

		*********************************************************
		STORMY'S LEADERBOARDS MESS
		*********************************************************
		
*/
/*
// I needed to encapsulate this in a class

// Dive away from an explosion for reduced damage 250 times
class CStormyLeaderboards : public CBaseAchievement
{
protected:
	virtual void Init()
	{
		SetFlags(ACH_SAVE_GLOBAL);
		SetGoal(1);
		m_bStoreProgressInSteam = true;
		//steamapicontext->SteamUserStats()->ResetAllStats(true);
	}

	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("event_DA_LEADERBOARD");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{

		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "event_DA_LEADERBOARD") && C_BasePlayer::GetLocalPlayer())
		{
			int iUserID = event->GetInt("userid"); // the userID passed from the event data
			// if the userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				//IncrementCount(); // WE ALL GOOD!


				// load all the event data into this class/object to be used by the leaderboard object
				
			/*	int ldb_lifetimeKills = event->GetInt("ldb_lifetimeKills");
				int ldb_brawlKills = event->GetInt("ldb_brawlKills");
				int ldb_diveshots = event->GetInt("ldb_diveshots");
				int ldb_diveKills = event->GetInt("ldb_diveKills");
				int ldb_divepunchKills = event->GetInt("ldb_divepunchKills");
				int ldb_divepunches = event->GetInt("ldb_divepunches");
				int ldb_slideshots = event->GetInt("ldb_slideshots");
				int ldb_slideKills = event->GetInt("ldb_slideKills");
				int ldb_slidepunchKills = event->GetInt("ldb_slidepunchKills");
				int ldb_slidepunches = event->GetInt("ldb_slidepunches");
				int ldb_headshotKills = event->GetInt("ldb_headshotKills");
				int ldb_headshots = event->GetInt("ldb_headshots");
				int lbd_punchesThrown = event->GetInt("lbd_punchesThrown");
				int ldb_punchesLanded = event->GetInt("ldb_punchesLanded");
				int ldb_totalStyle = event->GetInt("ldb_totalStyle");		*/
/*
				g_pSteamLeaderboards->FindLeaderboard("ldb_lifetimeKills");

			}
		}
	}
};
DECLARE_ACHIEVEMENT(CStormyLeaderboards, DODGETHIS, "LEADERBOARDHACK", 1)

*/

class CAchievementLeaderboardHack : public CSteamLeaderboards
{
public:
	void OnFindLeaderboard(LeaderboardFindResult_t *pCallback, bool bIOFailure);
	void FindLeaderboard(const char *pchLeaderboardName);
	bool UploadScore(int score);
	bool DownloadScores();
	
	CCallResult<CAchievementLeaderboardHack, LeaderboardFindResult_t> callResultFindLeaderboard;


	SteamLeaderboard_t curLeaderboard; // Handle to leaderboard


	CAchievementLeaderboardHack();
	~CAchievementLeaderboardHack(){};

protected:
	// register our event listeners
	virtual void ListenForEvents()
	{
		ListenForGameEvent("event_DA_LEADERBOARD");
	}

	// define what happens when we catch an event
	void FireGameEvent_Internal(IGameEvent *event)
	{

		
		// compare event names and check that we have a local player
		if (0 == Q_strcmp(event->GetName(), "event_DA_LEADERBOARD") && C_BasePlayer::GetLocalPlayer())
		{
			DevMsg("leaderboard event recieved by da_achievements");
			int iUserID = event->GetInt("userid"); // the userID passed from the event data

			// if the userID from the event matches the local player
			if (iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID())
			{
				//int ldb_lifetimeKills = event->GetInt("ldb_lifetimeKills");
				/*int ldb_brawlKills = event->GetInt("ldb_brawlKills");
				int ldb_diveshots = event->GetInt("ldb_diveshots");
				int ldb_diveKills = event->GetInt("ldb_diveKills");
				int ldb_divepunchKills = event->GetInt("ldb_divepunchKills");
				int ldb_divepunches = event->GetInt("ldb_divepunches");
				int ldb_slideshots = event->GetInt("ldb_slideshots");
				int ldb_slideKills = event->GetInt("ldb_slideKills");
				int ldb_slidepunchKills = event->GetInt("ldb_slidepunchKills");
				int ldb_slidepunches = event->GetInt("ldb_slidepunches");
				int ldb_headshotKills = event->GetInt("ldb_headshotKills");
				int ldb_headshots = event->GetInt("ldb_headshots");
				int lbd_punchesThrown = event->GetInt("lbd_punchesThrown");
				int ldb_punchesLanded = event->GetInt("ldb_punchesLanded");
				int ldb_totalStyle = event->GetInt("ldb_totalStyle");
				*/
				// find the leaderboard to upload our stat
				g_pSteamLeaderboards->FindLeaderboard("ldb_lifetimeKills");
				// get the existing value
				//				SteamAPICall_t currentLifetimeKills = steamapicontext->SteamUserStats()->DownloadLeaderboardEntries(lifetimeKillsLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
				//				steamapicontext->SteamUserStats()->UploadLeaderboardScore(lifetimeKillsLeaderboard, k_ELeaderboardUploadScoreMethodForceUpdate, currentLifetimeKills + ldb_lifetimeKills, NULL, 1);
				//DevMsg("Leaderboard event recieved! Lifetime Kills is %i \n", ldb_lifetimeKills);
			}
		}
	}
};



//DECLARE_ACHIEVEMENT(CAchievementLeaderboardHack, DODGETHIS, "event_DA_LEADERBOARD", 1)




//#endif // GAME_DLL
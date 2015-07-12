
#include "cbase.h"
#include "steamLeaderboards.h"
#include "../steam/isteamuserstats.h"
#include "../steam/steam_api.h"
#include "igameevents.h"
#include "c_da_player.h"


CSteamLeaderboards::CSteamLeaderboards() :
m_CurrentLeaderboard(NULL),
m_nLeaderboardEntries(0)
{
}


void CSteamLeaderboards::FindLeaderboard(const char *pchLeaderboardName)
{
	m_CurrentLeaderboard = NULL;

	DevMsg("searching for leaderboard %s\n", pchLeaderboardName);

	SteamAPICall_t hSteamAPICall = steamapicontext->SteamUserStats()->FindLeaderboard(pchLeaderboardName);
	m_callResultFindLeaderboard.Set(hSteamAPICall, this, &CSteamLeaderboards::OnFindLeaderboard);
}


void CSteamLeaderboards::OnFindLeaderboard(LeaderboardFindResult_t *pCallback, bool bIOFailure)
{
	// see if we encountered an error during the call
	if (!pCallback->m_bLeaderboardFound || bIOFailure)
	{
		DevMsg("Leaderboard could not be found\n");
		return;
	}
	else
	{
		DevMsg("Leaderboard found!\n");

//		C_DAPlayer* DAPlayer = C_DAPlayer::GetLocalDAPlayer();

		m_CurrentLeaderboard = pCallback->m_hSteamLeaderboard;

		if (0 == Q_strcmp(steamapicontext->SteamUserStats()->GetLeaderboardName(m_CurrentLeaderboard), "ldb_lifetimeKills")){
			// download the leaderboard data, and handle the rest of this flow in the OnDownloadScore callback
			DownloadScores();
			//addToScore(DAPlayer->ldb_lifetimeKills);
		}

		DevMsg("Leaderboard Info:\n");
		DevMsg("\tName: %s\n", steamapicontext->SteamUserStats()->GetLeaderboardName(m_CurrentLeaderboard));
		DevMsg("\tDisplay Type: %i\n", (int)steamapicontext->SteamUserStats()->GetLeaderboardDisplayType(m_CurrentLeaderboard));
		DevMsg("\tSort Method: %i\n", (int)steamapicontext->SteamUserStats()->GetLeaderboardSortMethod(m_CurrentLeaderboard));
		DevMsg("\tEntry Count: %i\n", steamapicontext->SteamUserStats()->GetLeaderboardEntryCount(m_CurrentLeaderboard));
	}
}


bool CSteamLeaderboards::UploadScore(int score)
{
	if (!m_CurrentLeaderboard)
	{
		DevMsg("ERROR: trying to upload score but the current leaderboard variable is null");
		return false;
	}
	else {

//		SteamAPICall_t downloadedEntries = steamapicontext->SteamUserStats()->DownloadLeaderboardEntries(
//			m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);



		SteamAPICall_t hSteamAPICall =
			steamapicontext->SteamUserStats()->UploadLeaderboardScore(m_CurrentLeaderboard, k_ELeaderboardUploadScoreMethodForceUpdate, score, NULL, 0);

		m_callResultUploadScore.Set(hSteamAPICall, this, &CSteamLeaderboards::OnUploadScore);

		return true;
	}
}


void CSteamLeaderboards::OnUploadScore(LeaderboardScoreUploaded_t *pCallback, bool bIOFailure)
{
	if (!pCallback->m_bSuccess || bIOFailure)
	{
		DevMsg("Score could not be uploaded to Steam\n");
	}
	else
		DevMsg("Score uploaded to Steam!\n");
}


bool CSteamLeaderboards::DownloadScores()
{
	if (!m_CurrentLeaderboard)
		return false;

	// load the specified leaderboard data around the current user
	SteamAPICall_t hSteamAPICall = steamapicontext->SteamUserStats()->DownloadLeaderboardEntries(
		m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -4, 5);
	m_callResultDownloadScore.Set(hSteamAPICall, this,
		&CSteamLeaderboards::OnDownloadScore);

	return true;
}


void CSteamLeaderboards::OnDownloadScore(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure)
	{
		//m_nLeaderboardEntries = min(pCallback->m_cEntryCount, 10);

		LeaderboardEntry_t hLeaderboardEntry;
		if (steamapicontext->SteamUserStats()->GetDownloadedLeaderboardEntry(pCallback->m_hSteamLeaderboardEntries, 0, &hLeaderboardEntry, 0, 0))
		{
			DevMsg("leaderboard shit downloaded: %i", hLeaderboardEntry.m_nScore);
			C_DAPlayer* C_DAPlayer = C_DAPlayer->GetLocalDAPlayer();
			DevMsg("leaderboard lifetime kills: %i\n", hLeaderboardEntry.m_nScore);
			DevMsg("Amount to add: %i\n", C_DAPlayer->ldb_lifetimeKills);
			DevMsg("Amount uploading: %i\n", hLeaderboardEntry.m_nScore + C_DAPlayer->ldb_lifetimeKills);

			UploadScore(hLeaderboardEntry.m_nScore + C_DAPlayer->ldb_lifetimeKills);
			// update and upload our score

		}
		/*
		for (int index = 0; index < m_nLeaderboardEntries; index++)
		{
			steamapicontext->SteamUserStats()->GetDownloadedLeaderboardEntry(
				pCallback->m_hSteamLeaderboardEntries, index, &m_leaderboardEntries[index], NULL, 0);

		}*/
	}
}

void  CSteamLeaderboards::FireGameEvent(IGameEvent *event)
{

	const char *name = event->GetName();
	if (0 == Q_strcmp(name, "teamplay_round_win"))
	{
		// if this is a round win and the achievement wants full round events only, filter this out
		// if this is not the end of a full round
		//if ((m_iFlags & ACH_FILTER_FULL_ROUND_ONLY) && (false == event->GetBool("full_round")))
			return;
	}

	// let the achievement handle the event
	FireGameEvent_Internal(event);
}



// can I create a class here?

CSteamLeaderboards* leaderboardsManager = new CSteamLeaderboards();

// register our event listeners



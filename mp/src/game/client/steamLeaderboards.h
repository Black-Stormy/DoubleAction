
#include "../steam/isteamuserstats.h"
#include "../steam/steam_api.h"
#include "igameevents.h"


class CSteamLeaderboards : public CGameEventListener
{
private:
	SteamLeaderboard_t m_CurrentLeaderboard; // Handle to leaderboard

public:
	int m_nLeaderboardEntries; // How many entries do we have?
	LeaderboardEntry_t m_leaderboardEntries[10]; // The entries

	CSteamLeaderboards();
	~CSteamLeaderboards(){};

	void FindLeaderboard(const char *pchLeaderboardName);
	bool UploadScore(int score);
	bool DownloadScores();
	void ListenForEvents();
	virtual void FireGameEvent(IGameEvent *event);
	virtual void FireGameEvent_Internal(IGameEvent *event) {};

	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardFindResult_t> m_callResultFindLeaderboard;

	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoreUploaded_t>  m_callResultUploadScore;

	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoresDownloaded_t>  m_callResultDownloadScore;


};
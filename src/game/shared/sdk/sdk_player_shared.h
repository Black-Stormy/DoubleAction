//============ Copyright � 1996-2008, Valve Corporation, All rights reserved. ===============//
//
// Purpose: Shared Player Variables / Functions and variables that may or may not be networked
//
//===========================================================================================//

#ifndef SDK_PLAYER_SHARED_H
#define SDK_PLAYER_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "networkvar.h"
#include "weapon_sdkbase.h"

#ifdef CLIENT_DLL
class C_SDKPlayer;
#else
class CSDKPlayer;
#endif

class CSDKPlayerShared
{
public:

#ifdef CLIENT_DLL
	friend class C_SDKPlayer;
	typedef C_SDKPlayer OuterClass;
	DECLARE_PREDICTABLE();
#else
	friend class CSDKPlayer;
	typedef CSDKPlayer OuterClass;
#endif

	DECLARE_EMBEDDED_NETWORKVAR()
	DECLARE_CLASS_NOBASE( CSDKPlayerShared );

	CSDKPlayerShared();
	~CSDKPlayerShared();

#if defined ( SDK_USE_STAMINA ) || defined ( SDK_USE_SPRINTING )
	void	SetStamina( float stamina );
	float	GetStamina( void ) { return m_flStamina; }
#endif // SDK_USE_STAMINA || SDK_USE_SPRINTING

	void	Init( OuterClass *pOuter );

	bool	IsSniperZoomed( void ) const;
	bool	IsDucking( void ) const; 

#if defined ( SDK_USE_PLAYERCLASSES )
	void	SetDesiredPlayerClass( int playerclass );
	int		DesiredPlayerClass( void );

	void	SetPlayerClass( int playerclass );
	int		PlayerClass( void );
#endif

	CWeaponSDKBase* GetActiveSDKWeapon() const;

#if defined ( SDK_USE_PRONE )
	void	StartGoingProne( void );
	void	StandUpFromProne( void );
	bool	IsProne() const;
	bool	IsGettingUpFromProne() const;	
	bool	IsGoingProne() const;
	void	SetProne( bool bProne, bool bNoAnimation = false );
	bool	CanChangePosition( void ) const;
#endif

	bool	IsSliding() const;
	bool	CanSlide() const;
	bool	IsGettingUpFromSlide() const;	
	void	StartSliding();
	void	EndSlide();
	void	StandUpFromSlide();
	float	GetSlideTime() const { return m_flSlideTime; };
	Vector	GetSlideDirection() const { return m_vecSlideDirection; };

	void	SetDuckPress(bool bReset = false);
	float	GetLastDuckPress() const { return m_flLastDuckPress; };
	bool	IsRolling() const;
	bool	CanRoll() const;
	void	StartRolling();
	void	EndRoll();
	float	GetRollTime() const { return m_flRollTime; };
	Vector	GetRollDirection() const { return m_vecRollDirection; };

	bool	IsDiving() const;
	bool	CanDive() const;
	Vector	StartDiving();
	void	EndDive();
	Vector	GetDiveDirection() const { return m_vecDiveDirection; };

	bool	IsJumping( void ) { return m_bJumping; }
	void	SetJumping( bool bJumping );

	void	ForceUnzoom( void );

#ifdef SDK_USE_SPRINTING
	bool	IsSprinting( void ) { return m_bIsSprinting; }

	void	SetSprinting( bool bSprinting );
	void	StartSprinting( void );
	void	StopSprinting( void );

	void ResetSprintPenalty( void );
#endif

	void ComputeWorldSpaceSurroundingBox( Vector *pVecWorldMins, Vector *pVecWorldMaxs );

private:

#if defined ( SDK_USE_PRONE )
	CNetworkVar( bool, m_bProne );
#endif

#if defined ( SDK_USE_PLAYERCLASSES )
	CNetworkVar( int, m_iPlayerClass );
	CNetworkVar( int, m_iDesiredPlayerClass );
#endif

#if defined ( SDK_USE_SPRINTING )
	CNetworkVar( bool, m_bIsSprinting );
	bool m_bGaveSprintPenalty;
#endif

	CNetworkVar( bool, m_bSliding );
	CNetworkVar( Vector, m_vecSlideDirection );
	CNetworkVar( float, m_flSlideTime );

	CNetworkVar( float, m_flLastDuckPress );
	CNetworkVar( bool, m_bRolling );
	CNetworkVar( Vector, m_vecRollDirection );
	CNetworkVar( float, m_flRollTime );

	CNetworkVar( bool, m_bDiving );
	CNetworkVar( Vector, m_vecDiveDirection );
	float m_flViewTilt;

#if defined ( SDK_USE_STAMINA ) || defined ( SDK_USE_SPRINTING )
	CNetworkVar( float, m_flStamina );
#endif // SDK_USE_STAMINA || SDK_USE_SPRINTING

public:

#ifdef SDK_USE_PRONE
	float m_flNextProneCheck; // Prevent it switching their prone state constantly.

	CNetworkVar( float, m_flUnProneTime );
	CNetworkVar( float, m_flGoProneTime );
	CNetworkVar( bool, m_bProneSliding );
#endif

	CNetworkVar( float, m_flUnSlideTime );
	CNetworkVar( Vector, m_vecUnSlideEyeStartOffset );

	bool m_bJumping;

	float m_flLastViewAnimationTime;

	//Tony; player speeds; at spawn server and client update both of these based on class (if any)
	float m_flRunSpeed;
	float m_flSprintSpeed;
	float m_flProneSpeed;
	float m_flSlideSpeed;
	float m_flRollSpeed;

private:

	OuterClass *m_pOuter;
};			   




#endif //SDK_PLAYER_SHARED_H
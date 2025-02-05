//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_dabase.h"
#include "da_gamerules.h"
#include "npcevent.h"
#include "engine/IEngineSound.h"
#include "weapon_basedagrenade.h"
#include "in_buttons.h"	


#ifdef CLIENT_DLL
	#include "c_da_player.h"
#else
	#include "da_player.h"
	#include "items.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define GRENADE_RADIUS	2.0f // inches


IMPLEMENT_NETWORKCLASS_ALIASED( BaseDAGrenade, DT_BaseDAGrenade )

BEGIN_NETWORK_TABLE(CBaseDAGrenade, DT_BaseDAGrenade)

#ifndef CLIENT_DLL
	SendPropBool( SENDINFO(m_bRedraw) ),
	SendPropBool( SENDINFO(m_bPinPulled) ),
	SendPropFloat( SENDINFO(m_fThrowTime), 0, SPROP_NOSCALE ),
#else
	RecvPropBool( RECVINFO(m_bRedraw) ),
	RecvPropBool( RECVINFO(m_bPinPulled) ),
	RecvPropFloat( RECVINFO(m_fThrowTime) ),
#endif

END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CBaseDAGrenade )
	DEFINE_PRED_FIELD( m_bRedraw, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
	DEFINE_PRED_FIELD( m_bRedraw, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_basedagrenade, CBaseDAGrenade );


CBaseDAGrenade::CBaseDAGrenade()
{
	m_bRedraw = false;
	m_bPinPulled = false;
	m_fThrowTime = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseDAGrenade::Precache()
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseDAGrenade::Deploy()
{
	m_bRedraw = false;
	m_bPinPulled = false;
	m_fThrowTime = 0;

	return BaseClass::Deploy();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CBaseDAGrenade::Holster( CBaseCombatWeapon *pSwitchingTo )
{
	m_bRedraw = false;
	m_bPinPulled = false; // when this is holstered make sure the pin isn�t pulled.
	m_fThrowTime = 0;

#ifndef CLIENT_DLL
	// If they attempt to switch weapons before the throw animation is done, 
	// allow it, but kill the weapon if we have to.
	CDAPlayer *pPlayer = GetPlayerOwner();
	
	if( pPlayer )
	{
		if( pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
		{
			CBaseCombatCharacter *pOwner = (CBaseCombatCharacter *)pPlayer;
			pOwner->Weapon_Drop( this );
			UTIL_Remove(this);
		}
	}
#endif

	return BaseClass::Holster( pSwitchingTo );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseDAGrenade::PrimaryAttack()
{
	if ( m_bRedraw || m_bPinPulled )
		return;

	CDAPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if ( pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
		return;

	// The pull pin animation has to finish, then we wait until they aren't holding the primary
	// attack button, then throw the grenade.
	SendWeaponAnim( ACT_VM_PULLPIN );
	m_bPinPulled = true;
	
	// Don't let weapon idle interfere in the middle of a throw!
	SetWeaponIdleTime( GetCurrentTime() + SequenceDuration() );

	m_flNextPrimaryAttack	= GetCurrentTime() + SequenceDuration();
	m_flNextSecondaryAttack	= GetCurrentTime() + SequenceDuration();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseDAGrenade::SecondaryAttack()
{
	BaseClass::SecondaryAttack();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CBaseDAGrenade::Reload()
{
	if ( ( m_bRedraw ) && ( m_flNextPrimaryAttack <= GetCurrentTime() ) && ( m_flNextSecondaryAttack <= GetCurrentTime() ) )
	{
		//Redraw the weapon
		SendWeaponAnim( ACT_VM_DRAW );

		//Update our times
		//Tony; updated; minimum grenade tossing time is 1 second delay! + sequence
		m_flNextPrimaryAttack	= GetCurrentTime() + SequenceDuration() + 1.0;
		m_flNextSecondaryAttack	= GetCurrentTime() + SequenceDuration() + 1.0;

		SetWeaponIdleTime( GetCurrentTime() + SequenceDuration() );
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseDAGrenade::ItemPostFrame()
{
	CDAPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	CBaseViewModel *vm = pPlayer->GetViewModel( m_nViewModelIndex );
	if ( !vm )
		return;

	// If they let go of the fire button, they want to throw the grenade.
	if ( m_bPinPulled && !(pPlayer->m_nButtons & IN_ATTACK) ) 
	{
		pPlayer->DoAnimationEvent( PLAYERANIMEVENT_ATTACK_PRIMARY );
//		if (m_bSecondary)
//			DropGrenade();
//		else
			ThrowGrenade();

		if (!pPlayer->IsStyleSkillActive(SKILL_TROLL))
			DecrementAmmo( pPlayer );

		m_bPinPulled = false;
		SendWeaponAnim( ACT_VM_THROW );	
		SetWeaponIdleTime( GetCurrentTime() + SequenceDuration() );

		m_bPinPulled = false;
//		m_bSecondary = false;
	}
	else if( m_bRedraw )
	{
		// Has the throw animation finished playing
		if( m_flTimeWeaponIdle < GetCurrentTime() )
		{
			// if we're officially out of grenades, ditch this weapon
			if( pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
			{
#ifdef GAME_DLL
				pPlayer->Weapon_Drop( this, NULL, NULL );
				UTIL_Remove(this);
#endif
				pPlayer->SwitchToNextBestWeapon( NULL ); //Tony; now switch! cuz we rans outs!
			}
			else if (pPlayer->IsStyleSkillActive(SKILL_TROLL))
			{
				m_bRedraw = false;
				m_flNextPrimaryAttack = GetCurrentTime() + 1.2;
				m_flNextSecondaryAttack = GetCurrentTime() + 1.2;
				SendWeaponAnim( GetDeployActivity() );	
			}
			else
			{
				m_bRedraw = false;

				// Only switch to the next best weapon if the next best weapon is not brawl.
				CBaseCombatWeapon *pNewWeapon = g_pGameRules->GetNextBestWeapon(pPlayer, this);
				CWeaponDABase* pSDKNewWeapon = dynamic_cast<CWeaponDABase*>(pNewWeapon);

				bool bSwitch = true;

				if (!pSDKNewWeapon)
					bSwitch = false;

				// If I'm going to switch to brawl but I have more grenades, don't switch.
				else if (pSDKNewWeapon && pSDKNewWeapon->GetWeaponID() == DA_WEAPON_BRAWL && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) > 0)
					bSwitch = false;

				if (bSwitch)
					pPlayer->SwitchToNextBestWeapon( this );
			}
			return;	//don't animate this grenade any more!
		}	
	}
	else if( !m_bRedraw )
	{
		BaseClass::ItemPostFrame();
	}
}



#ifdef CLIENT_DLL

	void CBaseDAGrenade::DecrementAmmo( CBaseCombatCharacter *pOwner )
	{
	}

	void CBaseDAGrenade::DropGrenade()
	{
		m_bRedraw = true;
		m_fThrowTime = 0.0f;
		//Tony; updated; minimum grenade tossing time is 1 second delay! + sequence
		m_flNextPrimaryAttack	= GetCurrentTime() + SequenceDuration() + 1.0;
		m_flNextSecondaryAttack	= GetCurrentTime() + SequenceDuration() + 1.0;
	}

	void CBaseDAGrenade::ThrowGrenade()
	{
		m_bRedraw = true;
		m_fThrowTime = 0.0f;
		//Tony; updated; minimum grenade tossing time is 1 second delay! + sequence
		m_flNextPrimaryAttack	= GetCurrentTime() + SequenceDuration() + 1.0;
		m_flNextSecondaryAttack	= GetCurrentTime() + SequenceDuration() + 1.0;
	}

	void CBaseDAGrenade::StartGrenadeThrow()
	{
		m_fThrowTime = GetCurrentTime() + 0.1f;
	}

#else

	BEGIN_DATADESC( CBaseDAGrenade )
		DEFINE_FIELD( m_bRedraw, FIELD_BOOLEAN ),
	END_DATADESC()

	int CBaseDAGrenade::CapabilitiesGet()
	{
		return bits_CAP_WEAPON_RANGE_ATTACK1; 
	}

	//-----------------------------------------------------------------------------
	// Purpose: 
	// Input  : *pOwner - 
	//-----------------------------------------------------------------------------
	void CBaseDAGrenade::DecrementAmmo( CBaseCombatCharacter *pOwner )
	{
		if( pOwner )
			pOwner->RemoveAmmo( 1, m_iPrimaryAmmoType );
	}

	void CBaseDAGrenade::StartGrenadeThrow()
	{
		m_fThrowTime = GetCurrentTime() + 0.1f;
	}
	// check a throw from vecSrc.  If not valid, move the position back along the line to vecEye
	void CBaseDAGrenade::CheckThrowPosition( CBasePlayer *pPlayer, const Vector &vecEye, Vector &vecSrc )
	{
		trace_t tr;

		if( pPlayer )
			UTIL_TraceHull( vecEye, vecSrc, -Vector(GRENADE_RADIUS+2,GRENADE_RADIUS+2,GRENADE_RADIUS+2), Vector(GRENADE_RADIUS+2,GRENADE_RADIUS+2,GRENADE_RADIUS+2), 
				pPlayer->PhysicsSolidMaskForEntity(), pPlayer, pPlayer->GetCollisionGroup(), &tr );

		if ( tr.DidHit() )
		{
			vecSrc = tr.endpos;
		}
	}

	void CBaseDAGrenade::ThrowGrenade()
	{
		CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
		if ( !pPlayer )
		{
			Assert( false );
			return;
		}

		Vector vecSrc, vecThrow;
		QAngle angThrow;
		GetGrenadeThrowVectors(vecSrc, vecThrow, angThrow);

		EmitGrenade( vecSrc, vec3_angle, vecThrow, AngularImpulse(600,random->RandomInt(-1200,1200),0), pPlayer, this );

		m_bRedraw = true;
		m_fThrowTime = 0.0f;
	}

	void CBaseDAGrenade::DropGrenade()
	{
		CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
		if ( !pPlayer )
		{
			Assert( false );
			return;
		}

		// BUGBUG: Hardcoded grenade width of 4 - better not change the model :)
		Vector vecSrc;
		pPlayer->CollisionProp()->NormalizedToWorldSpace( Vector( 0.5f, 0.5f, 0.0f ), &vecSrc );
		vecSrc.z += GRENADE_RADIUS;

		Vector vecFacing = pPlayer->BodyDirection2D( );
		// no up/down direction
		vecFacing.z = 0;
		VectorNormalize( vecFacing );
		trace_t tr;
		UTIL_TraceLine( vecSrc, vecSrc - Vector(0,0,16), MASK_PLAYERSOLID, pPlayer, COLLISION_GROUP_NONE, &tr );
		if ( tr.fraction != 1.0 )
		{
			// compute forward vec parallel to floor plane and roll grenade along that
			Vector tangent;
			CrossProduct( vecFacing, tr.plane.normal, tangent );
			CrossProduct( tr.plane.normal, tangent, vecFacing );
		}
		vecSrc += (vecFacing * 18.0);
		CheckThrowPosition( pPlayer, pPlayer->WorldSpaceCenter(), vecSrc );

		Vector vecThrow;
		pPlayer->GetVelocity( &vecThrow, NULL );
		vecThrow += vecFacing * 300;
		// put it on its side
		QAngle orientation(0,pPlayer->GetLocalAngles().y,-90);
		// roll it
		AngularImpulse rotSpeed(0,0,320);

		EmitGrenade( vecSrc, orientation, vecThrow, rotSpeed, pPlayer, this );

		m_bRedraw = true;
		m_fThrowTime = 0.0f;
	}

	void CBaseDAGrenade::EmitGrenade( Vector vecSrc, QAngle vecAngles, Vector vecVel, AngularImpulse angImpulse, CBasePlayer *pPlayer, CWeaponDABase *pWeapon )
	{
		Assert( 0 && "CBaseDAGrenade::EmitGrenade should not be called. Make sure to implement this in your subclass!\n" );
	}

	bool CBaseDAGrenade::AllowsAutoSwitchFrom( void ) const
	{
		return !m_bPinPulled;
	}

#endif


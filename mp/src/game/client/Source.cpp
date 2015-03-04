#include "cbase.h"
#include "server/triggers.h"

class CMyBrushEntity : public CBaseTrigger
{
public:
	DECLARE_CLASS(CMyBrushEntity, CBaseTrigger);
	DECLARE_DATADESC();

	void Spawn();

	void BrushTouch(CBaseEntity *pOther);
};

LINK_ENTITY_TO_CLASS(my_brush_entity, CMyBrushEntity);

BEGIN_DATADESC(CMyBrushEntity)

// Declare this function as being the touch function
DEFINE_ENTITYFUNC(BrushTouch),

END_DATADESC()

void CMyBrushEntity::Spawn()
{
	BaseClass::Spawn();

	// We want to capture touches from other entities
	SetTouch(&CMyBrushEntity::BrushTouch);

	// We should collide with physics
	SetSolid(SOLID_NONE);

	// Use our brushmodel
	SetModel(STRING(GetModelName()));
	
	// Create our physics hull information
	VPhysicsInitShadow(false, false);
}

void CMyBrushEntity::BrushTouch(CBaseEntity *pOther)
{
	// Get the collision information
	const trace_t &tr = GetTouchTrace();

	// We want to move away from the impact point along our surface
	Vector	vecPushDir = tr.plane.normal;
	vecPushDir.Negate();
	vecPushDir.z = 0.0f;

	// Uncomment this line to print plane information to the console in developer mode
	//DevMsg ( "%s (%s) touch plane's normal: [%f %f]\n", GetClassname(), GetDebugName(),tr.plane.normal.x, tr.plane.normal.y );

	// Move slowly in that direction
	LinearMove(GetAbsOrigin() + (vecPushDir * 64.0f), 32.0f);
}
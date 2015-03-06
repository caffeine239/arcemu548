/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _OBJECT_H
#define _OBJECT_H

class Unit;
class Group;


enum HIGHGUID_TYPE
{
	HIGHGUID_TYPE_PLAYER = 0x0000,
	HIGHGUID_TYPE_WAYPOINT = 0x1000,
	HIGHGUID_TYPE_ITEM = 0x4000,
	HIGHGUID_TYPE_CONTAINER = 0x4000,
	HIGHGUID_TYPE_GAMEOBJECT = 0xF110,
	HIGHGUID_TYPE_TRANSPORTER = 0xF120,
	HIGHGUID_TYPE_CREATURE = 0xF130,
	HIGHGUID_TYPE_UNIT = 0xF130,
	HIGHGUID_TYPE_PET = 0xF140,
	HIGHGUID_TYPE_VEHICLE = 0xF150,
	HIGHGUID_TYPE_DYNAMICOBJECT = 0xF100,
	HIGHGUID_TYPE_CORPSE = 0xF101,
	HIGHGUID_TYPE_AREATRIGGER = 0xF102,
	HIGHGUID_TYPE_MO_TRANSPORT = 0x1FC0,
	HIGHGUID_TYPE_GROUP = 0x1F50,
	HIGHGUID_TYPE_GUILD = 0x1FF6,
	//===============================================
	HIGHGUID_TYPE_MASK = 0xFFFF0000,
	LOWGUID_ENTRY_MASK = 0x0000FFFF,
};

#define GET_TYPE_FROM_GUID(x) ( Arcemu::Util::GUID_HIPART( (x) ) & HIGHGUID_TYPE_MASK )
#define GET_LOWGUID_PART(x) ( Arcemu::Util::GUID_LOPART( (x) ) & LOWGUID_ENTRY_MASK )

#define MAX_INTERACTION_RANGE 5.0f

// TODO: fix that type mess

enum TYPE
{
	TYPE_OBJECT = 0x0001,
	TYPE_ITEM = 0x0002,
	TYPE_CONTAINER = 0x0006,                       // TYPEMASK_ITEM | 0x0004
	TYPE_UNIT = 0x0008,                       // creature
	TYPE_PLAYER = 0x0010,
	TYPE_GAMEOBJECT = 0x0020,
	TYPE_DYNAMICOBJECT = 0x0040,
	TYPE_CORPSE = 0x0080,
	TYPE_AREATRIGGER = 0x0100,
	TYPE_SEER = TYPE_PLAYER | TYPE_UNIT | TYPE_DYNAMICOBJECT
};

enum TYPEID
{
	TYPEID_OBJECT = 0,
	TYPEID_ITEM = 1,
	TYPEID_CONTAINER = 2,
	TYPEID_UNIT = 3,
	TYPEID_PLAYER = 4,
	TYPEID_GAMEOBJECT = 5,
	TYPEID_DYNAMICOBJECT = 6,
	TYPEID_CORPSE = 7,
	TYPEID_AREATRIGGER = 8,
	TYPEID_SCENEOBJECT = 9
};

#define NUM_CLIENT_OBJECT_TYPES             10

uint32 GuidHigh2TypeId(uint32 guid_hi);

enum OBJECT_UPDATE_TYPE
{
	UPDATETYPE_VALUES = 0,
	UPDATETYPE_CREATE_OBJECT = 1,
	UPDATETYPE_CREATE_OBJECT2 = 2,
	UPDATETYPE_OUT_OF_RANGE_OBJECTS = 3,
};

enum PHASECOMMANDS
{
    PHASE_SET = 0, //overwrites the phase value with the supplied one
    PHASE_ADD = 1, //adds the new bits to the current phase value
    PHASE_DEL = 2, //removes the given bits from the current phase value
    PHASE_RESET = 3 //sets the default phase of 1, same as PHASE_SET with 1 as the new value
};

typedef struct
{
	uint32 school_type;
	int32 full_damage;
	uint32 resisted_damage;
} dealdamage;

struct TransporterInfo{
	uint64 guid;
	float x;
	float y;
	float z;
	float o;
	uint32 flags;
	uint8 seat;

	TransporterInfo(){
		guid = 0;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		o = 0.0f;
		flags = 0;
		seat = 0;
	}
};


struct Position
{
	struct PositionXYZStreamer
	{
		explicit PositionXYZStreamer(Position& pos) : m_pos(&pos) { }
		Position* m_pos;
	};

	struct PositionXYZOStreamer
	{
		explicit PositionXYZOStreamer(Position& pos) : m_pos(&pos) { }
		Position* m_pos;
	};

	float m_positionX;
	float m_positionY;
	float m_positionZ;
	// Better to limit access to m_orientation field, but this will be hard to achieve with many scripts using array initialization for this structure
private:
	float m_orientation;
public:

	bool operator==(Position const &a);

	inline bool operator!=(Position const &a)
	{
		return !(operator==(a));
	}

	void Relocate(float x, float y)
	{
		m_positionX = x; m_positionY = y;
	}
	void Relocate(float x, float y, float z)
	{
		m_positionX = x; m_positionY = y; m_positionZ = z;
	}
	void Relocate(float x, float y, float z, float orientation)
	{
		m_positionX = x; m_positionY = y; m_positionZ = z; SetOrientation(orientation);
	}
	void Relocate(Position const &pos)
	{
		m_positionX = pos.m_positionX; m_positionY = pos.m_positionY; m_positionZ = pos.m_positionZ; SetOrientation(pos.m_orientation);
	}
	void Relocate(Position const* pos)
	{
		m_positionX = pos->m_positionX; m_positionY = pos->m_positionY; m_positionZ = pos->m_positionZ; SetOrientation(pos->m_orientation);
	}
	void RelocateOffset(Position const &offset);
	void SetOrientation(float orientation)
	{
		m_orientation = NormalizeOrientation(orientation);
	}

	float GetPositionX() const { return m_positionX; }
	float GetPositionY() const { return m_positionY; }
	float GetPositionZ() const { return m_positionZ; }
	float GetOrientation() const { return m_orientation; }

	void GetPosition(float &x, float &y) const
	{
		x = m_positionX; y = m_positionY;
	}
	void GetPosition(float &x, float &y, float &z) const
	{
		x = m_positionX; y = m_positionY; z = m_positionZ;
	}
	void GetPosition(float &x, float &y, float &z, float &o) const
	{
		x = m_positionX; y = m_positionY; z = m_positionZ; o = m_orientation;
	}
	void GetPosition(Position* pos) const
	{
		if (pos)
			pos->Relocate(m_positionX, m_positionY, m_positionZ, m_orientation);
	}

	Position::PositionXYZStreamer PositionXYZStream()
	{
		return PositionXYZStreamer(*this);
	}
	Position::PositionXYZOStreamer PositionXYZOStream()
	{
		return PositionXYZOStreamer(*this);
	}

	bool IsPositionValid() const;

	float GetExactDist2dSq(float x, float y) const
	{
		float dx = m_positionX - x; float dy = m_positionY - y; return dx*dx + dy*dy;
	}
	float GetExactDist2d(const float x, const float y) const
	{
		return sqrt(GetExactDist2dSq(x, y));
	}
	float GetExactDist2dSq(Position const* pos) const
	{
		float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; return dx*dx + dy*dy;
	}
	float GetExactDist2d(Position const* pos) const
	{
		return sqrt(GetExactDist2dSq(pos));
	}
	float GetExactDistSq(float x, float y, float z) const
	{
		float dz = m_positionZ - z; return GetExactDist2dSq(x, y) + dz*dz;
	}
	float GetExactDist(float x, float y, float z) const
	{
		return sqrt(GetExactDistSq(x, y, z));
	}
	float GetExactDistSq(Position const* pos) const
	{
		float dx = m_positionX - pos->m_positionX; float dy = m_positionY - pos->m_positionY; float dz = m_positionZ - pos->m_positionZ; return dx*dx + dy*dy + dz*dz;
	}
	float GetExactDist(Position const* pos) const
	{
		return sqrt(GetExactDistSq(pos));
	}

	void GetPositionOffsetTo(Position const & endPos, Position & retOffset) const;

	float GetAngle(Position const* pos) const;
	float GetAngle(float x, float y) const;
	float GetRelativeAngle(Position const* pos) const
	{
		return GetAngle(pos) - m_orientation;
	}
	float GetRelativeAngle(float x, float y) const { return GetAngle(x, y) - m_orientation; }
	void GetSinCos(float x, float y, float &vsin, float &vcos) const;

	bool IsInDist2d(float x, float y, float dist) const
	{
		return GetExactDist2dSq(x, y) < dist * dist;
	}
	bool IsInDist2d(Position const* pos, float dist) const
	{
		return GetExactDist2dSq(pos) < dist * dist;
	}
	bool IsInDist(float x, float y, float z, float dist) const
	{
		return GetExactDistSq(x, y, z) < dist * dist;
	}
	bool IsInDist(Position const* pos, float dist) const
	{
		return GetExactDistSq(pos) < dist * dist;
	}
	bool HasInArc(float arcangle, Position const* pos, float border = 2.0f) const;
	//bool HasInLine(WorldObject const* target, float width) const;
	std::string ToString() const;

	// modulos a radian orientation to the range of 0..2PI
	static float NormalizeOrientation(float o)
	{
		// fmod only supports positive numbers. Thus we have
		// to emulate negative numbers
		if (o < 0)
		{
			float mod = o *-1;
			mod = fmod(mod, 2.0f * static_cast<float>(M_PI));
			mod = -mod + 2.0f * static_cast<float>(M_PI);
			return mod;
		}
		return fmod(o, 2.0f * static_cast<float>(M_PI));
	}
};

struct MovementInfo3
{
	// common
	uint64 guid;
	uint32 flags;
	uint16 flags2;
	Position pos;
	uint32 time;

	// transport
	struct TransportInfo
	{
		void Reset()
		{
			guid = 0;
			pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
			seat = -1;
			time = 0;
			time2 = 0;
			time3 = 0;
		}

		uint64 guid;
		Position pos;
		int8 seat;
		uint32 time;
		uint32 time2;
		uint32 time3;
	} transport;

	// swimming/flying
	float pitch;

	// jumping
	struct JumpInfo
	{
		void Reset()
		{
			fallTime = 0;
			zspeed = sinAngle = cosAngle = xyspeed = 0.0f;
		}

		uint32 fallTime;

		float zspeed, sinAngle, cosAngle, xyspeed;

	} jump;

	// spline
	float splineElevation;

	MovementInfo3() :
		guid(0), flags(0), flags2(0), time(0), pitch(0.0f), splineElevation(0.0f)
	{
		pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
		transport.Reset();
		jump.Reset();
	}

	uint32 GetMovementFlags() const { return flags; }
	void SetMovementFlags(uint32 flag) { flags = flag; }
	void AddMovementFlag(uint32 flag) { flags |= flag; }
	void RemoveMovementFlag(uint32 flag) { flags &= ~flag; }
	bool HasMovementFlag(uint32 flag) const { return flags & flag; }

	uint16 GetExtraMovementFlags() const { return flags2; }
	void SetExtraMovementFlags(uint16 flag) { flags2 = flag; }
	void AddExtraMovementFlag(uint16 flag) { flags2 |= flag; }
	void RemoveExtraMovementFlag(uint16 flag) { flags2 &= ~flag; }
	bool HasExtraMovementFlag(uint16 flag) const { return flags2 & flag; }

	void SetFallTime(uint32 time) { jump.fallTime = time; }

	void ResetTransport()
	{
		transport.Reset();
	}

	void ResetJump()
	{
		jump.Reset();
	}

	void OutDebug();
};

class WorldPacket;
class ByteBuffer;
class WorldSession;
class Player;
class MapCell;
class MapMgr;
class ObjectContainer;
class DynamicObject;
class Creature;
class GameObject;
class Unit;
class Group;
class Pet;
class Spell;



//====================================================================
//  Object
//  Base object for every item, unit, player, corpse, container, etc
//====================================================================
class SERVER_DECL Object : public EventableObject
{
	public:
		typedef std::set<Object*> InRangeSet;
		typedef std::map<string, void*> ExtensionSet;

		virtual ~Object();

		virtual void Init();

		virtual void Update(uint32 time) { }

		//! True if object exists in world, else false
		bool IsInWorld() { return m_mapMgr != NULL; }
		virtual void AddToWorld();
		virtual void AddToWorld(MapMgr* pMapMgr);
		void PushToWorld(MapMgr*);
		virtual void RemoveFromWorld(bool free_guid);


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnPrePushToWorld()
		//  Virtual method that is called, BEFORE pushing the Object in the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnPrePushToWorld() {}


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnPushToWorld()
		//  Virtual method that is called, AFTER pushing the Object in the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnPushToWorld() {}


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnPreRemoveFromWorld()
		//  Virtual method that is called, BEFORE removing the Object from the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnPreRemoveFromWorld() {}


		////////////////////////////////////////////////////////////////////////////////
		//virtual void OnRemoveFromWorld()
		//  Virtual method that is called, AFTER removing the Object from the game world
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		////////////////////////////////////////////////////////////////////////////////
		virtual void OnRemoveFromWorld() {}



		//! Guid always comes first
		
		
		ARCEMU_INLINE const uint64& GetGUID() const { return *((uint64*)m_uint32Values); }
		void SetGUID(uint64 GUID) { SetUInt64Value(OBJECT_FIELD_GUID, GUID); }
		void SetLowGUID(uint32 val) { m_uint32Values[0] = val; }
		void SetHighGUID(uint32 val) { m_uint32Values[1] = val; }

		ARCEMU_INLINE const WoWGuid& GetNewGUID() const { return m_wowGuid; }
		ARCEMU_INLINE uint32 GetEntry(){ return m_uint32Values[OBJECT_FIELD_ENTRY]; }
		void SetEntry(uint32 value) { SetUInt32Value(OBJECT_FIELD_ENTRY, value); }

		float GetScale() { return m_floatValues[ OBJECT_FIELD_SCALE_X ]; }
		void SetScale(float scale) { SetFloatValue(OBJECT_FIELD_SCALE_X, scale); };

		ARCEMU_INLINE uint32 GetEntryFromGUID() const  { return GUID_ENPART_TEST(GetGUID()); }
		ARCEMU_INLINE uint32 GetTypeFromGUID() const { return GUID_HIPAR_TESTT(GetGUID()); }
		ARCEMU_INLINE uint32 GetUIdFromGUID() const { return GUID_LOPART_TEST(GetGUID()); }
		ARCEMU_INLINE uint32 GetHighGUID() const { return GUID_HIPAR_TESTT(GetGUID()); }
		ARCEMU_INLINE uint32 GetLowGUID() const { return GUID_LOPART_TEST(GetGUID()); }




		// type
		const uint8 & GetTypeId() const { return m_objectTypeId; }
		bool IsUnit()	{ return (m_objectTypeId == TYPEID_UNIT || m_objectTypeId == TYPEID_PLAYER); }
		bool IsPlayer() { return m_objectTypeId == TYPEID_PLAYER; }
		bool IsCreature() { return m_objectTypeId == TYPEID_UNIT; }
		bool IsItem() { return m_objectTypeId == TYPEID_ITEM; }
		virtual bool IsPet() { return false; }
		virtual bool IsTotem() { return false; }
		virtual bool IsSummon() { return false; }
		virtual bool IsVehicle(){ return false; }
		bool IsGameObject() { return m_objectTypeId == TYPEID_GAMEOBJECT; }
		bool IsCorpse() { return m_objectTypeId == TYPEID_CORPSE; }
		bool IsContainer() { return m_objectTypeId == TYPEID_CONTAINER; }

		//! This includes any nested objects we have, inventory for example.
		virtual uint32  BuildCreateUpdateBlockForPlayer(ByteBuffer* data, Player* target);
		uint32  BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, Player* target);
		uint32  BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, UpdateMask* mask);
		uint32  BuildOutOfRangeUpdateBlock(ByteBuffer* buf);

		WorldPacket* BuildFieldUpdatePacket(uint32 index, uint32 value);
		void BuildFieldUpdatePacket(Player* Target, uint32 Index, uint32 Value);
		void BuildFieldUpdatePacket(ByteBuffer* buf, uint32 Index, uint32 Value);

		virtual void DealDamage(Unit* pVictim, uint32 damage, uint32 targetEvent, uint32 unitEvent, uint32 spellId, bool no_remove_auras = false);
		
		bool SetPosition(float newX, float newY, float newZ, float newOrientation, bool allowPorting = false);

		bool SetPosition2(const Position &pos, bool allowPorting = false);
		bool SetPosition(const LocationVector & v, bool allowPorting = false);

		const float & GetPositionX() const { return m_position.x; }
		const float & GetPositionY() const { return m_position.y; }
		const float & GetPositionZ() const { return m_position.z; }
		const float & GetOrientation() const { return m_position.o; }
		void SetOrientation(float o) { m_position.o = o; }

		const float & GetSpawnX() const { return m_spawnLocation.x; }
		const float & GetSpawnY() const { return m_spawnLocation.y; }
		const float & GetSpawnZ() const { return m_spawnLocation.z; }
		const float & GetSpawnO() const { return m_spawnLocation.o; }

		const LocationVector & GetPosition() { return m_position; }
		LocationVector & GetPositionNC() { return m_position; }
		LocationVector* GetPositionV() { return &m_position; }

		//! Distance Calculation
		float CalcDistance(Object* Ob);
		float CalcDistance(float ObX, float ObY, float ObZ);
		float CalcDistance(Object* Oa, Object* Ob);
		float CalcDistance(Object* Oa, float ObX, float ObY, float ObZ);
		float CalcDistance(float OaX, float OaY, float OaZ, float ObX, float ObY, float ObZ);
		//! NYS: scriptdev2
		bool IsInMap(Object* obj) { return GetMapId() == obj->GetMapId() && GetInstanceID() == obj->GetInstanceID(); }
		bool IsWithinDistInMap(Object* obj, const float dist2compare) const;
		bool IsWithinLOSInMap(Object* obj);
		bool IsWithinLOS(LocationVector location);

		//! Only for MapMgr use
		MapCell* GetMapCell() const;
		const uint32 GetMapCellX() { return m_mapCell_x; }
		const uint32 GetMapCellY() { return m_mapCell_y; }
		//! Only for MapMgr use
		void SetMapCell(MapCell* cell);
		//! Only for MapMgr use
		MapMgr* GetMapMgr() const { return m_mapMgr; }

		Object* GetMapMgrObject(const uint64 & guid);
		Pet* GetMapMgrPet(const uint64 & guid);
		Unit* GetMapMgrUnit(const uint64 & guid);
		Player* GetMapMgrPlayer(const uint64 & guid);
		Creature* GetMapMgrCreature(const uint64 & guid);
		GameObject* GetMapMgrGameObject(const uint64 & guid);
		DynamicObject* GetMapMgrDynamicObject(const uint64 & guid);

		void SetMapId(uint32 newMap) { m_mapId = newMap; }
		void SetZoneId(uint32 newZone);

		const uint32 GetMapId() const { return m_mapId; }
		const uint32 & GetZoneId() const { return m_zoneId; }

		//! Get uint32 property
		const uint32 & GetUInt32Value(uint32 index) const
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_uint32Values[ index ];
		}

		const uint64 & GetUInt64Value(uint32 index) const
		{
			ARCEMU_ASSERT(index + uint32(1) < m_valuesCount);

			uint64* p = reinterpret_cast< uint64* >(&m_uint32Values[ index ]);

			return *p;
		}

		bool isType(uint16 mask) const { return (mask & m_objectType); }

		//! Get float property
		const float & GetFloatValue(uint32 index) const
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_floatValues[ index ];
		}

		void  ModFloatValue(const uint32 index, const float value);
		void ModFloatValueByPCT(const uint32 index, int32 byPct);
		void ModSignedInt32Value(uint32 index, int32 value);
		void ModUnsigned32Value(uint32 index, int32 mod);
		uint32 GetModPUInt32Value(const uint32 index, const int32 value);

		//! Set uint32 property
		void SetByte(uint32 index, uint32 index1, uint8 value);

		uint8 GetByte(uint32 i, uint32 i1)
		{
			ARCEMU_ASSERT(i < m_valuesCount);
			ARCEMU_ASSERT(i1 < 4);
			return ((uint8*)m_uint32Values)[i * 4 + i1];
		}

		uint8 GetByteValue(uint16 index, uint8 offset) const
		{
			ASSERT(index < m_valuesCount);
			ASSERT(offset < 4);
			return *(((uint8*)&m_uint32Values[index]) + offset);
		}

		void  SetByteFlag(uint16 index, uint8 offset, uint8 newFlag);
		void  RemoveByteFlag(uint16 index, uint8 offset, uint8 newFlag);

		bool HasByteFlag(uint32 index, uint32 index1, uint8 flag)
		{
			return ((GetByte(index, index1) & flag) != 0);
		}

		void SetNewGuid(uint32 Guid)
		{
			SetLowGUID(Guid);
			m_wowGuid.Init(GetGUID());
		}

		void EventSetUInt32Value(uint32 index, uint32 value);
		void  SetUInt32Value(const uint32 index, const uint32 value);

		//! Set uint64 property
		void  SetUInt64Value(const uint32 index, const uint64 value);

		//! Set float property
		void  SetFloatValue(const uint32 index, const float value);

		void  SetFlag(const uint32 index, uint32 newFlag);

		void  RemoveFlag(const uint32 index, uint32 oldFlag);

		uint32 HasFlag(const uint32 index, uint32 flag) const
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_uint32Values[ index ] & flag;
		}

		////////////////////////////////////////
		void ClearUpdateMask()
		{
			m_updateMask.Clear();
			m_objectUpdated = false;
		}

		bool HasUpdateField(uint32 index)
		{
			ARCEMU_ASSERT(index < m_valuesCount);
			return m_updateMask.GetBit(index);
		}

		//! Use this to check if a object is in range of another
		bool isInRange(Object* target, float range);

		//! Use this to Check if a object is in front of another object.
		bool isInFront(Object* target);
		//! Use this to Check if a object is in back of another object.
		bool isInBack(Object* target);
		//! Check to see if an object is in front of a target in a specified arc (in degrees)
		bool isInArc(Object* target , float degrees);
		//! NYS: Scriptdev2
		bool HasInArc(float degrees, Object* target);
		//! Calculates the angle between two positions
		float calcAngle(float Position1X, float Position1Y, float Position2X, float Position2Y);
		float calcRadAngle(float Position1X, float Position1Y, float Position2X, float Position2Y);

		//! Converts to 360 > x > 0
		float getEasyAngle(float angle);

		const float GetDistanceSq(Object* obj)
		{
			if(obj->GetMapId() != m_mapId) return 40000.0f; //enough for out of range
			return m_position.DistanceSq(obj->GetPosition());
		}

		float GetDistanceSq(LocationVector & comp)
		{
			return comp.DistanceSq(m_position);
		}

		float CalcDistance(LocationVector & comp)
		{
			return comp.Distance(m_position);
		}

		const float GetDistanceSq(float x, float y, float z)
		{
			return m_position.DistanceSq(x, y, z);
		}

		const float GetDistance2dSq(Object* obj)
		{
			if(obj->GetMapId() != m_mapId)
				return 40000.0f; //enough for out of range
			return m_position.Distance2DSq(obj->m_position);
		}

		// In-range object management, not sure if we need it
		bool IsInRangeSet(Object* pObj)
		{
			return !(m_objectsInRange.find(pObj) == m_objectsInRange.end());
		}

		virtual void AddInRangeObject(Object* pObj);

		Mutex m_inrangechangelock;

		void RemoveInRangeObject(Object* pObj);

		//////////////////////////////////////////////////////////////////////
		//void RemoveSelfFromInrangeSets()
		// Removes the Object from the inrangesets of the Objects in range
		//
		// Parameters:
		//  None
		//
		// Return Value:
		//  None
		//
		//
		///////////////////////////////////////////////////////////////////////
		void RemoveSelfFromInrangeSets();

		bool HasInRangeObjects()
		{
			return (m_objectsInRange.size() > 0);
		}

		virtual void OnRemoveInRangeObject(Object* pObj);

		virtual void ClearInRangeSet()
		{
			m_objectsInRange.clear();
			m_inRangePlayers.clear();
			m_oppFactsInRange.clear();
			m_sameFactsInRange.clear();
		}

		size_t GetInRangeCount() { return m_objectsInRange.size(); }
		size_t GetInRangePlayersCount() { return m_inRangePlayers.size();}
		InRangeSet::iterator GetInRangeSetBegin() { return m_objectsInRange.begin(); }
		InRangeSet::iterator GetInRangeSetEnd() { return m_objectsInRange.end(); }
		InRangeSet::iterator FindInRangeSet(Object* obj) { return m_objectsInRange.find(obj); }

		void RemoveInRangeObject(InRangeSet::iterator itr)
		{
			OnRemoveInRangeObject(*itr);
			m_objectsInRange.erase(itr);
		}

		bool RemoveIfInRange(Object* obj)
		{
			InRangeSet::iterator itr = m_objectsInRange.find(obj);
			if(obj->IsPlayer())
				m_inRangePlayers.erase(obj);

			if(itr == m_objectsInRange.end())
				return false;

			m_objectsInRange.erase(itr);

			return true;
		}

		bool IsInRangeSameFactSet(Object* pObj) { return (m_sameFactsInRange.count(pObj) > 0); }
		void UpdateSameFactionSet();
		std::set<Object*>::iterator GetInRangeSameFactsSetBegin() { return m_sameFactsInRange.begin(); }
		std::set<Object*>::iterator GetInRangeSameFactsSetEnd() { return m_sameFactsInRange.end(); }

		bool IsInRangeOppFactSet(Object* pObj) { return (m_oppFactsInRange.count(pObj) > 0); }
		void UpdateOppFactionSet();
		size_t GetInRangeOppFactsSize() { return m_oppFactsInRange.size(); }
		std::set<Object*>::iterator GetInRangeOppFactsSetBegin() { return m_oppFactsInRange.begin(); }
		std::set<Object*>::iterator GetInRangeOppFactsSetEnd() { return m_oppFactsInRange.end(); }
		std::set<Object*>::iterator GetInRangePlayerSetBegin() { return m_inRangePlayers.begin(); }
		std::set<Object*>::iterator GetInRangePlayerSetEnd() { return m_inRangePlayers.end(); }
		std::set<Object*> * GetInRangePlayerSet() { return &m_inRangePlayers; }

		std::set<Object*> & GetInRangePlayers() { return m_inRangePlayers; }

		std::set<Object*> & GetInRangeOpposingFactions() { return m_oppFactsInRange; }

		std::set<Object*> & GetInRangeSameFactions() { return m_sameFactsInRange; }

		std::set<Object*> & GetInRangeObjects() { return m_objectsInRange; }



		///////////////////////////////////////////////////////////////////////////
		//void OutPacket( uint32 opcode, uint16 len, const void *data )
		// Sends a packet to the Player
		//
		//
		// Parameters
		//  uint32 opcode      -   opcode of the packet
		//  uint16 len         -   length/size of the packet
		//  const void *data   -   the data that needs to be sent
		//
		//
		// Return value
		//  none
		//
		//////////////////////////////////////////////////////////////////////////
		virtual void OutPacket(uint32 opcode, uint16 len, const void* data) {};



		/////////////////////////////////////////////////////////////////////////
		//void SendPacket( WorldPacket *packet )
		// Sends a packet to the Player
		//
		// Parameters
		//  WorldPAcket *packet      -     the packet that needs to be sent
		//
		//
		// Return value
		//  none
		//
		////////////////////////////////////////////////////////////////////////
		virtual void SendPacket(WorldPacket* packet) {};

		virtual void SendMessageToSet(WorldPacket* data, bool self, bool myteam_only = false);
		void SendMessageToSet(StackBufferBase* data, bool self) { OutPacketToSet(data->GetOpcode(), static_cast<uint16>(data->GetSize()), data->GetBufferPointer(), self); }
		virtual void OutPacketToSet(uint32 opcode, uint16 Len, const void* Data, bool self);

		/////////////////////////////////////////////////////////////////////////
		//void SendAIReaction( uint32 reaction = 2 )
		//  Notifies the player's clients about the AI reaction of this object
		//  ( NPC growl for example "aggro sound" )
		//
		//Parameter(s)
		//  uint32 reaction  -  Reaction type
		//
		//Return Value
		//  None
		//
		//
		/////////////////////////////////////////////////////////////////////////
		void SendAIReaction(uint32 reaction = 2);

		/////////////////////////////////////////////////////////////////////////
		//void SendDestroyObject()
		//  Destroys this Object for the players' clients that are nearby
		//  ( removes object from the scene )
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		/////////////////////////////////////////////////////////////////////////
		void SendDestroyObject();

		//! Fill values with data from a space separated string of uint32s.
		void LoadValues(const char* data);

		uint16 GetValuesCount() const { return m_valuesCount; }

		//! Blizzard seem to send those for all object types. weird.
		float m_walkSpeed;
		float m_runSpeed;
		float m_backWalkSpeed;
		float m_swimSpeed;
		float m_backSwimSpeed;
		float m_turnRate;
		float m_flySpeed;
		float m_backFlySpeed;

		float m_base_runSpeed;
		float m_base_walkSpeed;

		TransporterInfo transporter_info;
		MovementInfo3 m_movementInfo;

		uint32 m_phase; //This stores the phase, if two objects have the same bit set, then they can see each other. The default phase is 0x1.

		const uint32 GetPhase() { return m_phase; }
		virtual void Phase(uint8 command = PHASE_SET, uint32 newphase = 1);

		void EventSpellDamage(uint64 Victim, uint32 SpellID, uint32 Damage);
		void SpellNonMeleeDamageLog(Unit* pVictim, uint32 spellID, uint32 damage, bool allowProc, bool static_damage = false, bool no_remove_auras = false);
		virtual bool IsCriticalDamageForSpell(Object* victim, SpellEntry* spell) { return false; }
		virtual float GetCriticalDamageBonusForSpell(Object* victim, SpellEntry* spell, float amount) { return 0; }
		virtual bool IsCriticalHealForSpell(Object* victim, SpellEntry* spell) { return false; }
		virtual float GetCriticalHealBonusForSpell(Object* victim, SpellEntry* spell, float amount) { return 0; }

		//*****************************************************************************************
		//* SpellLog packets just to keep the code cleaner and better to read
		//*****************************************************************************************
		void SendSpellLog(Object* Caster, Object* Target, uint32 Ability, uint8 SpellLogType);
		void SendSpellNonMeleeDamageLog(Object* Caster, Object* Target, uint32 SpellID, uint32 Damage, uint8 School, uint32 AbsorbedDamage, uint32 ResistedDamage, bool PhysicalDamage, uint32 BlockedDamage, bool CriticalHit, bool bToSet);
		void SendAttackerStateUpdate(Object* Caster, Object* Target, dealdamage* Dmg, uint32 Damage, uint32 Abs, uint32 BlockedDamage, uint32 HitStatus, uint32 VState);

		//object faction
		void _setFaction();
		uint32 _getFaction() {return m_faction->Faction;}

		FactionTemplateDBC* m_faction;
		FactionDBC* m_factionDBC;

		void SetInstanceID(int32 instance) { m_instanceId = instance; }
		int32 GetInstanceID() { return m_instanceId; }

		int32 event_GetInstanceID();

		// Object activation
	private:
		bool Active;
		ByteBuffer m_PackGUID;
	public:
		bool IsActive() { return Active; }
		virtual bool CanActivate();
		virtual void Activate(MapMgr* mgr);
		virtual void Deactivate(MapMgr* mgr);
		//! Player is in pvp queue.
		bool m_inQueue;
		void SetMapMgr(MapMgr* mgr) { m_mapMgr = mgr; }

		void Delete()
		{
			if(IsInWorld())
				RemoveFromWorld(true);
			delete this;
		}
		//! Play's a sound to players in range.
		void PlaySoundToSet(uint32 sound_entry);
		//! Is the player in a battleground?
		bool IsInBg();
		//! What's their faction? Horde/Ally.
		uint32 GetTeam();
		//! Objects directly cannot be in a group.
		virtual Group* GetGroup() { return NULL; }

		Player* ToPlayer() { if (GetTypeId() == TYPEID_PLAYER) return reinterpret_cast<Player*>(this); else return NULL; }
		Player const* ToPlayer() const { if (GetTypeId() == TYPEID_PLAYER) return reinterpret_cast<Player const*>(this); else return NULL; }

		Creature* ToCreature() { if (GetTypeId() == TYPEID_UNIT) return reinterpret_cast<Creature*>(this); else return NULL; }
		Creature const* ToCreature() const { if (GetTypeId() == TYPEID_UNIT) return reinterpret_cast<Creature const*>(this); else return NULL; }

		Unit* ToUnit() { if (isType(TYPE_UNIT)) return reinterpret_cast<Unit*>(this); else return NULL; }
		Unit const* ToUnit() const { if (isType(TYPE_UNIT)) return reinterpret_cast<Unit const*>(this); else return NULL; }

		GameObject* ToGameObject() { if (GetTypeId() == TYPEID_GAMEOBJECT) return reinterpret_cast<GameObject*>(this); else return NULL; }
		GameObject const* ToGameObject() const { if (GetTypeId() == TYPEID_GAMEOBJECT) return reinterpret_cast<GameObject const*>(this); else return NULL; }

		//Corpse* ToCorpse() { if (GetTypeId() == TYPEID_CORPSE) return reinterpret_cast<Corpse*>(this); else return NULL; }
		//Corpse const* ToCorpse() const { if (GetTypeId() == TYPEID_CORPSE) return reinterpret_cast<Corpse const*>(this); else return NULL; }

		DynamicObject* ToDynObject() { if (GetTypeId() == TYPEID_DYNAMICOBJECT) return reinterpret_cast<DynamicObject*>(this); else return NULL; }
		DynamicObject const* ToDynObject() const { if (GetTypeId() == TYPEID_DYNAMICOBJECT) return reinterpret_cast<DynamicObject const*>(this); else return NULL; }


	protected:
		Object();

		//void _Create (uint32 guidlow, uint32 guidhigh);
		void _Create(uint32 mapid, float x, float y, float z, float ang);

		//! Mark values that need updating for specified player.
		virtual void _SetUpdateBits(UpdateMask* updateMask, Player* target) const;
		//! Mark values that player should get when he/she/it sees object for first time.
		virtual void _SetCreateBits(UpdateMask* updateMask, Player* target) const;
		//! Create updates that player will see
		void _BuildMovementUpdate(ByteBuffer* data, uint16 flags, uint32 flags2, Player* target);
		void _BuildValuesUpdate(ByteBuffer* data, UpdateMask* updateMask, Player* target);

		//! WoWGuid class
		WoWGuid m_wowGuid;
		//! Type id.
		uint16 m_objectType;
		uint8 m_objectTypeId;
		uint16 m_updateFlag;
		//! Zone id.
		uint32 m_zoneId;
		//! Continent/map id.
		uint32 m_mapId;
		//! Map manager
		MapMgr* m_mapMgr;
		//! Current map cell row and column
		uint32 m_mapCell_x, m_mapCell_y;

		

		/* Main Function called by isInFront(); */
		bool inArc(float Position1X, float Position1Y, float FOV, float Orientation, float Position2X, float Position2Y);

		LocationVector m_position;
		LocationVector m_lastMapUpdatePosition;
		LocationVector m_spawnLocation;

		//! Object properties.
		union
		{
			uint32* m_uint32Values;
			float* m_floatValues;
		};

		//! Number of properties
		uint16 m_valuesCount;

		//! List of object properties that need updating.
		UpdateMask m_updateMask;

		//! True if object was updated
		bool m_objectUpdated;

		//! Set of Objects in range.
		//! TODO: that functionality should be moved into WorldServer.
		std::set<Object*> m_objectsInRange;
		std::set<Object*> m_inRangePlayers;
		std::set<Object*> m_oppFactsInRange;
		std::set<Object*> m_sameFactsInRange;


		int32 m_instanceId;

		ExtensionSet* m_extensions;

		// so we can set from scripts. :)
		void _SetExtension(const string & name, void* ptr);

	public:

		template<typename T>
		void SetExtension(const string & name, T ptr)
		{
			_SetExtension(name, ((void*)ptr));
		}

		template<typename T>
		T GetExtension(const string & name)
		{
			if(m_extensions == NULL)
				return ((T)NULL);
			else
			{
				ExtensionSet::iterator itr = m_extensions->find(name);
				if(itr == m_extensions->end())
					return ((T)NULL);
				else
					return ((T)itr->second);
			}
		}

		bool m_loadedFromDB;

		// Spell currently casting
		Spell* m_currentSpell;
		Spell* GetCurrentSpell() {return m_currentSpell;}
		void SetCurrentSpell(Spell* cSpell) { m_currentSpell = cSpell; }

		//Andy's crap
		virtual Object* GetPlayerOwner();
		std::set<Spell*> m_pendingSpells;

		bool GetPoint(float angle, float rad, float & outx, float & outy, float & outz, bool sloppypath = false);
		bool GetRandomPoint(float rad, float & outx, float & outy, float & outz) { return GetPoint(RandomFloat(float(M_PI * 2)), rad, outx, outy, outz); }
		bool GetRandomPoint(float rad, LocationVector & out) { return GetRandomPoint(rad, out.x, out.y, out.z); }

		uint16 m_map;
		uint32 m_blockCount;
		std::set<uint64> m_outOfRangeGUIDs;
		ByteBuffer m_data;
};


#endif




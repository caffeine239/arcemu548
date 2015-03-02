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

#include "StdAfx.h"

void WorldSession::HandleAttackSwingOpcode(WorldPacket & recv_data)
{
	ObjectGuid guid;

	guid[6] = recv_data.ReadBit();
	guid[5] = recv_data.ReadBit();
	guid[7] = recv_data.ReadBit();
	guid[0] = recv_data.ReadBit();
	guid[3] = recv_data.ReadBit();
	guid[1] = recv_data.ReadBit();
	guid[4] = recv_data.ReadBit();
	guid[2] = recv_data.ReadBit();

	recv_data.ReadByteSeq(guid[6]);
	recv_data.ReadByteSeq(guid[7]);
	recv_data.ReadByteSeq(guid[1]);
	recv_data.ReadByteSeq(guid[3]);
	recv_data.ReadByteSeq(guid[2]);
	recv_data.ReadByteSeq(guid[0]);
	recv_data.ReadByteSeq(guid[4]);
	recv_data.ReadByteSeq(guid[5]);

	sLog.outError("HandleAttackSwingOpcode guid : %u", guid);
	if(!guid)
	{
		GetPlayer()->EventAttackStop();
		GetPlayer()->smsg_AttackStop(guid);
		return;
	}

	// AttackSwing
	Log.Debug("WORLD", "Recvd CMSG_ATTACKSWING Message");

	if(GetPlayer()->IsPacified() || GetPlayer()->IsStunned() || GetPlayer()->IsFeared())
		return;

//	printf("Got ATTACK SWING: %08X %08X\n", GUID_HIPART(guid), Arcemu::Util::GUID_LOPART( guid ));
	Unit* pEnemy = _player->GetMapMgr()->GetUnit(guid);
	//printf("Pointer: %08X\n", pEnemy);

	if(!pEnemy)
	{
		LOG_ERROR("WORLD: " I64FMT " does not exist.", guid);
		return;
	}

	if(pEnemy->IsDead() || _player->IsDead() || !isAttackable(_player, pEnemy, false))		// haxors :(
		return;

	GetPlayer()->smsg_AttackStart(pEnemy);
	GetPlayer()->EventAttackStart();

}

void WorldSession::HandleAttackStopOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid = GetPlayer()->GetSelection();

	if(guid)
	{
		Unit* pEnemy = _player->GetMapMgr()->GetUnit(guid);
		if(pEnemy != NULL)
		{
			GetPlayer()->EventAttackStop();
			GetPlayer()->smsg_AttackStop(pEnemy);
		}
	}
}


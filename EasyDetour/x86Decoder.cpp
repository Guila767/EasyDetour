#include "x86Decoder.h"

constexpr unsigned char nextop(const unsigned char* ip) { return *(ip + 1); }

const singleOpcodeMap getSingleOpcodeMap(const unsigned char opcode)
{
	for (size_t i = 0; i < sizeof(single_opcode_map) / sizeof(single_opcode_map[0]); i++)
		if (single_opcode_map[i][OPCODE] == opcode)
			return (const unsigned int*)single_opcode_map[i];
	return (const unsigned int*)single_opcode_map[0];
}

bool decodeCall(const void* const retAddrs, PCallCommand pCallCmd)
{
	if (!retAddrs || !pCallCmd)
		return false;

#ifdef _M_AMD64
	signed short depth = 0xA;
#else
	signed short depth = 0x6;
#endif

	const unsigned char* ip = ((const unsigned char*)retAddrs - 0x1); // A 1 byte long call doesn't exist
	singleOpcodeMap map = nullptr;

	while (depth-- >= 0)
	{
		if (*ip == 0xff)
		{
			map = getSingleOpcodeMap(nextop(ip));
			switch (map[COMMAND])
			{
			case NONE:
			{
				break;
			}
			case CMD_PUSH:
			{
				if (((unsigned char*)retAddrs - ip) != 0x3)
					break;
				pCallCmd->type = NEAR_ABSOLUTE_CALL;
				pCallCmd->types.callMPTR =
				{
					map[OPERAND0],
					(int)(*(ip + 0x2))
				};
				return true;
			}
			case CMD_NOP:
			case CMD_XCHG:
			{
				if (((unsigned char*)retAddrs - ip) != 0x6)
					break;
				unsigned int reg;
				if (nextop(ip) == 0x90)
					reg = REG_EAX;
				else
					reg = map[OPERAND0];
				pCallCmd->type = NEAR_ABSOLUTE_CALL;
				pCallCmd->types.callMPTR =
				{
					reg,
					*((int*)(ip + 0x2))
				};
				return true;
			}
			}
			ip--;
			continue;
		}
		else if (*ip == 0xE8)
		{
#ifdef _M_AMD64
			//constexpr unsigned int cmdSize = 0x9;
			constexpr unsigned int cmdSize = 0x5;
#else
			constexpr unsigned int cmdSize = 0x5;
#endif
			if (((unsigned char*)retAddrs - ip) != cmdSize)
			{
				ip--;
				continue;
			}
			pCallCmd->type = NEAR_REALTIVE_CALL;
			return true;
		}
		ip--;
	}
	//pCallCmd->type = NONE;
	return false;
}

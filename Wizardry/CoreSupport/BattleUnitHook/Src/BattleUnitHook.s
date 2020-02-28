.thumb

.global UnitToBattleHook
.type   UnitToBattleHook, %function

.global BattleToUnitHook
.type   BattleToUnitHook, %function

.global UnitToBattleVanilla
.type   UnitToBattleVanilla, %function

.global BattleToUnitVanilla
.type   BattleToUnitVanilla, %function

UnitToBattleHook:
	@ arguments:
	@ r0 = battle unit (dest)
	@ r1 = unit (source)

	push {r4-r7, lr}

	ldr r6, =Unit2BattleFuncList

common:
	@ make sure none of the arguments are null pointers

	cmp r0, #0
	beq end

	cmp r1, #0
	beq end

	@ save loop address in r7, we gonna put that in lr later
	@ micro optimizations!

	adr r7, lop
	add r7, #1

	mov r4, r0 @ r4 = dest
	mov r5, r1 @ r5 = source

	.align @ we need the lop label to be at an aligned address, for adr

lop:
	@ load next routine
	ldmia r6!, {r3}

	@ end if null (that's the terminator, hopefully)
	cmp r3, #0
	beq end

	mov r0, r4 @ arg r0 = dest
	mov r1, r5 @ arg r1 = source

	@ call routine

	mov lr, r7
	bx  r3

end:
	pop {r4-r7}

	pop {r0}
	bx  r0

.pool
.align

BattleToUnitHook:
	@ arguments:
	@ r0 = unit (dest)
	@ r1 = battle unit (source)

	push {r4-r7, lr}

	ldr r6, =Battle2UnitFuncList

	@ This is actually the same exact logic as for UnitToBattleHook
	@ With only the function list that changes
	@ So we jump to that

	b common

.pool
.align

UnitToBattleVanilla:
	@ this is all replaced stuff
	@ 8 opcodes (0x10 bytes, the amount taken by a LynJump)

	@ well, it's 6 + one bl (which is actually 2 different opcodes)
	@ the bl was translated to a load + bl to bx which is why there's an extra opcode here

	push {r4-r6,lr}

	mov r5, r0
	mov r6, r1

	cmp r6, #0
	beq end

	mov r2, #0x48 @ arg r0 = size

	ldr r3, =memcpy
	bl  BXR3

	@ jump to the rest of the vanilla routine

	ldr r3, =(0x802A594+1)
BXR3:	bx  r3

.pool
.align

BattleToUnitVanilla:
	@ this is all replaced stuff
	@ 8 opcodes (0x10 bytes, the amount taken by a LynJump)

	push {r4-r6, lr}

	mov  r4, r0
	mov  r5, r1

	ldrb r0, [r5, #0x08]
	strb r0, [r4, #0x08]

	ldrb r0, [r5, #0x09]
	strb r0, [r4, #0x09]

	ldrb r0, [r5, #0x13]

	@ jump to the rest of the vanilla routine

	ldr r3, =(0x802C1FC+1)
	bx  r3

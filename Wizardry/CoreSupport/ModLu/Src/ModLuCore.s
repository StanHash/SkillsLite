
	.thumb

	.global MLUApplyPromotion
	.type   MLUApplyPromotion, function

	.global MLUCheckForLevelUp
	.type   MLUCheckForLevelUp, function

	.global MLUMakeBattleUnitPromoteGains
	.type   MLUMakeBattleUnitPromoteGains, function

	.global MLUStandardGetMaxLevel
	.type   MLUStandardGetMaxLevel, function

	.global MLUStandardStatGains
	.type   MLUStandardStatGains, function

	@ Global symbols it needs to work:
	@ - gpGetUnitMaxLevel (pointer to max level getter)
	@ - gLevelUpFuncList (null-terminated pointer list containing all routines called on level up)

	@ Unit struct field offset definitions
	Unit.pCharData  = 0x00 @ word
	Unit.pClassData = 0x04 @ word
	Unit.level      = 0x08 @ byte
	Unit.exp        = 0x09 @ byte
	Unit.aiFlags    = 0x0A @ byte
	Unit.index      = 0x0B @ byte
	Unit.state      = 0x0C @ word
	@ ...
	Unit.health     = 0x13 @ byte

	@ BattleUnit struct field offset definitions
	BattleUnit.expGained = 0x6E
	@ ... (am lazy)

	origin = 0x0802BC00

	GetClassData = . + 0x08019444 - origin
	GetUnitMaxHp = . + 0x08019190 - origin

	@ FIRST, here's the hook at 0802BC00, since we're going inline

MLUApplyUnitDefaultPromotion:
	ldr  r1, [r0, #4] @ r1 = class
	ldrb r1, [r1, #5] @ r1 = default promoted class

	@ fallthrough

	.align

MLUApplyPromotion:
	@ jump from 0802BD50

	@ ARGUMENTS:
	@	r0 = unit
	@	r1 = classId

	push {r4-r7, lr}

	mov r7, r0 @ var r7 = unit
	mov r6, r1 @ var r6 = classId

	@ r5 = return address for called functions
	adr r5, MLUApplyPromotion.loop
	add r5, #1

	@ r4 = list iterator
	ldr r4, =OnPromoteFuncList

	.align

MLUApplyPromotion.loop:
	@ r3 = *it++
	ldmia r4!, {r3}

	@ End the routine if we reached list terminator

	cmp r3, #0
	beq MLUApplyPromotion.finish

	@ "call" routine with r0 = bu pointer
	@ lr = MLUCheckForLevelUp.loop (with thumb bit)

	mov r0, r7 @ arg r0 = unit
	mov r1, r6 @ arg r1 = classId

	mov lr, r5
	bx  r3

MLUApplyPromotion.finish:
	@ Update unit class

	mov r0, r6 @ arg r0 = classId
	bl GetClassData

	str r0, [r7, #4] @ unit.class = GetClassData(classId);

	@ Cap unit health

	mov r0, r7 @ arg r0 = unit
	bl GetUnitMaxHp

	mov  r1, #Unit.health
	ldsb r1, [r7, r1]

	cmp r1, r0
	blt MLUApplyPromotion.end

	strb r0, [r7, #Unit.health]

MLUApplyPromotion.end:
	pop {r4-r7}

	pop {r0}
	bx  r0

	.align
	.pool

MLUCheckForLevelUp:
	@ jump from 0802BA4C
	@ Which is in the middle of CheckBattleUnitLevelUp

	@ KNOWN STATE:
	@	stack frame is 0x30 bytes (0x10 + 8 pushes)
	@	r7 = bu (BattleUnit)
	@	r3 = exp amount (over 100!)
	@	r4-r6 and r8-r10 are free

	@ Store new exp value

	sub  r3, #100
	strb r3, [r7, #Unit.exp]

	@ r4 = extra exp value (we may need it later)
	mov r4, r3

	@ Increment level
	@ r5 = incremented level

	ldrb r0, [r7, #Unit.level]
	add  r5, r0, #1
	strb r5, [r7, #Unit.level]

	@ Get max level

	mov r0, r7 @ arg r0 = unit

	ldr r3, =GetUnitMaxLevelFunc
	ldr r3, [r3]

	bl BXR3

	@ Compare level to max level

	cmp r0, r5
	bne MLUCheckForLevelUp.skip_cap_level

	@ Here our unit has reached its maximum level

	@ Update exp gained to not contain the extra bit

	mov  r1, #BattleUnit.expGained
	ldrb r0, [r7, r1]
	sub  r0, r4 @ expGained - (expTotal - 100)
	strb r0, [r7, r1]

	@ Set exp to max

	mov  r0, #0xFF
	strb r0, [r7, #Unit.exp]

MLUCheckForLevelUp.skip_cap_level:
	@ Here is where it gets interesting :)

	@ r6 = (MLUCheckForLevelUp.loop+1)
	@ we gonna put that into lr instead of using bl
	@ gotta love them micro optimizations

	adr r6, MLUCheckForLevelUp.loop
	add r6, #1

	@ r4 = list iterator

	ldr r4, =OnLevelUpFuncList

	.align

MLUCheckForLevelUp.loop:
	@ r3 = *it++
	ldmia r4!, {r3}

	@ End the routine if we reached list terminator

	cmp r3, #0
	beq MLUCheckForLevelUp.end

	@ "call" routine with r0 = bu pointer
	@ lr = MLUCheckForLevelUp.loop (with thumb bit)

	mov r0, r7

	mov lr, r6
BXR3:	bx  r3

MLUCheckForLevelUp.end:
	add sp, #0x10

	pop {r3-r5}
	mov r8,  r3
	mov r9,  r4
	mov r10, r5

	pop {r4-r7}

	pop {r0}
	bx  r0

	.pool
	.align

MLUMakeBattleUnitPromoteGains:
	push {r4-r7, lr}

	mov r7, r0 @ var r7 = arg r0 (after bu)
	mov r6, r0 @ var r6 = arg r1 (before bu)

	@ r5 = return address for called functions
	adr r5, MLUMakeBattleUnitPromoteGains.loop
	add r5, #1

	@ r4 = list iterator
	ldr r4, =MakePromoteGainsFuncList

	.align

MLUMakeBattleUnitPromoteGains.loop:
	@ r3 = *it++
	ldmia r4!, {r3}

	@ End the routine if we reached list terminator

	cmp r3, #0
	beq MLUMakeBattleUnitPromoteGains.end

	@ "call" routine with r0 = bu pointer
	@ lr = MLUCheckForLevelUp.loop (with thumb bit)

	mov r0, r7 @ arg r0 = after bu
	mov r1, r6 @ arg r1 = before bu

	mov lr, r5
	bx  r3

MLUMakeBattleUnitPromoteGains.end:
	pop {r4-r7}

	pop {r0}
	bx r0

	.pool
	.align

MLUStandardStatGains:
	@ this is just a wrapper around the second part of vanilla CheckBattleUnitLevelUp
	@ aka the part that does the stat gains

	push {r4-r7, lr}

	mov  r7, r10
	mov  r6, r9
	mov  r5, r8
	push {r5-r7}

	sub  sp, sp, #0x10

	mov r7, r0

	ldr r3, =(0x0802BA8A+1)
	bx  r3

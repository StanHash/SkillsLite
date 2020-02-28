
	.thumb

	.global UnitLoadHook
	.type   UnitLoadHook, function

UnitLoadHook:
	@ callHack from FE8U:08017CB8

	@ STATE:
	@	r5 = unit
	@	r6 = unit info

	push {r4, r7, lr}

	@ r7 = return address for called functions
	adr r7, UnitLoadHook.lop
	add r7, #1

	@ r4 = list iterator
	ldr r4, =OnLoadUnitFuncList

	.align

UnitLoadHook.lop:
	@ r3 = *it++
	ldmia r4!, {r3}

	@ End the routine if we reached list terminator

	cmp r3, #0
	beq UnitLoadHook.end

	@ "call" routine
	@ lr = UnitLoadHook.lop (with thumb bit)

	mov r0, r5 @ arg r0 = unit
	mov r1, r6 @ arg r1 = unit info

	mov lr, r7
	bx  r3

UnitLoadHook.end:
	pop {r4, r7}

	pop {r0}
	bx  r0

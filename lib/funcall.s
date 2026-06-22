LL0:
	.data
	.text
	.align	1
	.globl	_funcall
_funcall:
	link.l	fp,#L41
	movem.l	#L42,(sp)
	lea	(-4,sp),a0
	clr.w	(-2,fp)
L45:
	tst.l	(12,fp)
	jbeq	L44
	addq.w	#4,(-2,fp)
	suba.l	#4,sp
	move.l	sp,a1
LA1:
	cmp.l	a1,a0
	jbeq	LA2
	move.l	(4,a1),(a1)+
	jbra	LA1
LA2:
	move.l	([12,fp],4),(a1)
L43:
	move.l	([12,fp],8),(12,fp)
	jbra	L45
L44:
	move.l	(8,fp),d0
	andi.l	#65535,d0
	move.l	d0,d1
	asl.l	#4,d0
	asl.l	#2,d1
	add.l	d1,d0			` d0 *= sizeof(struct htable)
	lea	([_Htable],d0.l),a0
	jsr	([16,a0])
	add.l	(-2,fp),sp
L40:
	.set	L42,0x0
	unlk	fp
	rts
	.set	L41,-4
	.data

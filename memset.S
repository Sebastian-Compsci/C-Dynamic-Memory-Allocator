.text

.global memset

//
// x0 = ptr
// x1 = value
// x2 = size in bytes
//
memset:
	mov x3, x0
	cmp x2, #0
	beq done

loops:
	strb w1, [x0]
	add x0, x0, #1

	subs x2, x2, #1
	bne loops

done:
	mov x0, x3
	ret


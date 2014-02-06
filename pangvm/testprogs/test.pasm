main:
	add		$1
	cmp		$2
	jl		%main
	mul		$50
END:

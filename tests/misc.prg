#!/usr/local/bin/pewter

using standard

func main()
	array = ["zero" "one" "two" "three" "four" "five"]
	(output (obj array 2))
	(- array 2)
	(output (obj array 2))

	(output (read (open (input))))
end

#!/usr/local/bin/pewter

using standard

func main()
    f = (open (input))
    (output (input f))

    array = ["key 1" "object 2" ["asfd" "asdasdasd" 0]]
    (output (obj array 0))

    dict = {"key 1" "object 1" "key 2" "object 2"} # dictionary syntax
    k = (obj array 0)
    (output (obj dict k))
    (output (random))
    (wait 010)
    (output (random))
end

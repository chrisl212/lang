func pow2(n)
    (* n n)
end

#the 'main' function of this langauge.

func entry(argc argv)
    (output "Enter your name")

    name = (input 0)
    s2 = (+ "Hello, " name ", how is your day going?")
    (output s2)

    n = 532; q = (pow2 n); (output q); q = (- 5 4); (output q) # expressions can be separated by either a newline or semicolon

    arg = (idx argv 1) #idx is the current solution for 'object at index' -- likely to change
    (output arg)

    arg = 389429
    (output arg " Swag") #print takes an unlimited number of args

    array = ["object 1" "object 2" ["object 3"]] #nested literals
    o = (idx array 2)
    p = (idx o 0)
    (output p)
    # dictionary = {"key 1" "object 1" "key 2" "object 2"} dictionary syntax
end

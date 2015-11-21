using standard

func split(s)
    n = 0
    p = ""
    a = []
    while (< n (count s))
        p = (+ p (obj s n))

        if (= n (/ (count s) 2))
            (+ a (copy p))
            p = ""
        stop
        n = (+ n 1)
    stop
    a
end

func main()
    f = (open "/Users/christopherloonam/Desktop/text.txt")
    (output (input f))

#(output (^ 4 -4)) # expressions can be separated by either a newline or semicolon
    array = ["key 1" "object 2" ["asfd" "asdasdasd" 0]]
    (output (obj array 0))

    dict = {"key 1" "object 1" "key 2" "object 2"} # dictionary syntax
    k = (obj array 0)
    (output (obj dict k))
    (output (random))
    (wait 010)
    (output (random))

    p = (split "Hello, world!")
    (output (obj p 1))
end
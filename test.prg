func pow(m n)
    o = m
    if (= n 0)
        m = 1
    stop
    n = (- n 1)
    while (> n 0)
        m = (* m o)
        n = (- n 1)
    stop
    m
end

func main(argc argv)
    (output "Enter your name")

    q = (pow 4 2); (output (pow 4 (- 9 6))); # expressions can be separated by either a newline or semicolon

    (output "asd")

    arg = 389429
    (output arg " Swag") #print takes an unlimited number of args

    array = ["object 1" "object 2" "asf"] #nested literals
    (output (obj array 1))

    dict = {"key 1" "object 1" "key 2" "object 2"} # dictionary syntax
    (output (obj dict "key 1"))
end

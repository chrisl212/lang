#!/usr/local/bin/lang

func main(argc argv)
(output (+ "Hello, " (obj argv 2)))
    (output "How much beer would you like?")
    n = (int (input 0))
    while (> n 0)
        if (= n 1)
            (output n " bottle of beer on the wall.")
        stop
        if (!= n 1)
            (output n " bottles of beer on the wall.")
        stop

        n=(- n 1)
    stop
end

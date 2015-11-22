func output()
    n = 0
    while (< n (count args))
        a = (obj args n)
        (write a)
        n = (+ n 1)
        (write "\n")
    stop
end
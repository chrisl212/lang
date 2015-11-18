def cat(a1 a2)
    def res = (+ a1 a2)
    res #last statement in a function is its return value -- currently must be a variable name
end

def pow2(n)
def res = (* n n) #yet to implement for/while loops ...
    res
end

#the 'main' function of this langauge.

def entry(argc argv)
    def s1 = "Hello,"
    def s2 = " world! Why is this so much harder than imagined?"
    def s4 = " Why can't it just be easy?"
    def s5 = (+ s2 s4)
    def s3 = (cat s1 s5)
    (print s3)

    def n = 532
    def q = (pow2 n)
    (print q)

    def i = 0
def arg = (idx argv i) #idx is the current solution for 'object at index' -- likely to change
    (print arg)
end

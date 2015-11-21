class Range:
    """A class that mimics the built-in range class.
    Using lazy mechanism meaning that dont store the numbers
    interfaces are:  __len__, __getitem__,
    when it provides support for these two, it inherits automatic support for iteration ,which is why it is possible to execute a for loop over a Range"""
    def __init__(self,start,stop=None,step=1): #polymorphic methods
        """Initialize a range instance"""
#first do the input checking
        if step==0:
            raise ValueError("step cannot be 0")
        if stop is None: #special case of range(n) means the range(0,n)
            start,stop = 0,start

        #calculate the effective length once
        self._length = max(0,(stop-1-start+step)//step)
#need the start and step to provide getitem, but not stop 
        self._start = start
        self._step = step
    
    def __len__(self):
        """Return number of entries in the range"""
        return self._length
    def __getitem__(self,k):
        if k<0:
            k +=len(self) #attempt to convert negative index
        if not 0<= k <self._length:
            raise IndexError("index out of range")

        return self._start + k*self._step





if __name__ =="__main__":
#do unit testing
    print "Testing for only start"
    for i in Range(10):
        print i,
    print
    print "Testing for start and stop"
    for i in Range(5,15):
        print i,
    print
    print "Testing for 3 arguments"
    for i in Range(3,10,2):
        print i,
    print 

    print "Testing for ValueError "
  
    
    try:
        for i in Range(1,10,0):
            print i,
    except ValueError as e:
        print "Catch a value error with step==0"

    print "Testing for negative index should be 9"
    a = Range(10)
    print a[-1]
        

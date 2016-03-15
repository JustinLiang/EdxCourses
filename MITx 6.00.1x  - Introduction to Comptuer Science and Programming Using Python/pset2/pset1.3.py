abcStr = "abcdefghijklmnopqrstuvwxyz"
lastc = ""
longestStr1 = ""
longestStr2 = ""
for c in s:
    if abcStr.find(c) >= abcStr.find(lastc):
        longestStr1 = longestStr1 + c
    else:
        longestStr1 = c    
    if len(longestStr1) > len(longestStr2):
        longestStr2 = longestStr1  
    lastc = c
print "Longest substring in alphabetical order is: " + longestStr2
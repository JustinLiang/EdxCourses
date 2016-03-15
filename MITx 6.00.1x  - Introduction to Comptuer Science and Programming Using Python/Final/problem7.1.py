# -*- coding: utf-8 -*-
class Frob(object):
    def __init__(self, name):
        self.name = name
        self.before = None
        self.after = None
    def setBefore(self, before):
        # example: a.setBefore(b) sets b before a
        self.before = before
    def setAfter(self, after):
        # example: a.setAfter(b) sets b after a
        self.after = after
    def getBefore(self):
        return self.before
    def getAfter(self):
        return self.after
    def myName(self):
        return self.name
        
def insert(atMe, newFrob):
    """
    atMe: a Frob that is part of a doubly linked list
    newFrob:  a Frob with no links 
    This procedure appropriately inserts newFrob into the linked list that atMe is a part of.    
    """
    atMeBeg = atMe
    atMeEnd = atMe
    doneFlag = False
    
    "It is before the beginning?"
    while atMeBeg.getBefore() != None:
        atMeBeg = atMeBeg.getBefore()  
    
    if atMeBeg.name > newFrob.name:
        atMeBeg.setBefore(newFrob)
        newFrob.setAfter(atMeBeg)
        doneFlag = True      
        
    "Is it after the end?"
    while atMeEnd.getAfter() != None and doneFlag == False:
        atMeEnd = atMeEnd.getAfter()  
        
    if atMeEnd.name < newFrob.name and doneFlag == False:
        atMeEnd.setAfter(newFrob)
        newFrob.setBefore(atMeEnd)
        doneFlag = True
                  
    "It is somewhere in the middle."
    while True and doneFlag == False:
        if atMeBeg.getAfter().name < newFrob.name:
            atMeBeg = atMeBeg.getAfter()
        else:
            newFrob.setBefore(atMeBeg)
            newFrob.setAfter(atMeBeg.getAfter())
            atMeBeg.getAfter().setBefore(newFrob)
            atMeBeg.setAfter(newFrob)
            break
            
    
eric = Frob('eric')
andrew = Frob('andrew')
ruth = Frob('ruth')
fred = Frob('fred')
martha = Frob('martha')

insert(eric, andrew)
insert(eric, ruth)
insert(eric, fred)
insert(ruth, martha)    

test_list = Frob('abby')
insert(test_list, Frob("xander"))
insert(test_list, Frob("beto"))
eric
